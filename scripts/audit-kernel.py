"""Repeatable, PID-bound legacy bridge feasibility audit. Writes only test fixtures."""
from pathlib import Path
import ctypes
from ctypes import wintypes
import json
import queue
import shutil
import subprocess
import threading
import time

ROOT = Path(__file__).resolve().parents[1]
WORK = ROOT / 'workspace/kernel-audit'
LOGS = ROOT / 'logs/kernel-audit'
BRIDGE = ROOT / 'build/HostBridge.exe'
NO_WINDOW = subprocess.CREATE_NO_WINDOW


def creation_time(pid):
    api = ctypes.WinDLL('kernel32', use_last_error=True)
    api.OpenProcess.argtypes = [wintypes.DWORD, wintypes.BOOL, wintypes.DWORD]
    api.OpenProcess.restype = wintypes.HANDLE
    api.GetProcessTimes.argtypes = [wintypes.HANDLE] + [ctypes.POINTER(wintypes.FILETIME)] * 4
    api.CloseHandle.argtypes = [wintypes.HANDLE]
    handle = api.OpenProcess(0x1000, False, pid)
    if not handle:
        raise ProcessLookupError(pid)
    times = [wintypes.FILETIME() for _ in range(4)]
    try:
        if not api.GetProcessTimes(handle, *(ctypes.byref(x) for x in times)):
            raise OSError(ctypes.get_last_error())
        return str((times[0].dwHighDateTime << 32) | times[0].dwLowDateTime)
    finally:
        api.CloseHandle(handle)


def owned_host(engine):
    marker_path = LOGS / (engine + '-session.json')
    if marker_path.exists():
        marker = json.loads(marker_path.read_text(encoding='utf-8'))
        try:
            if creation_time(marker['pid']) == marker['createdFileTime']:
                return marker
        except (ProcessLookupError, OSError):
            pass
    args = [str(ROOT / 'OpenMSE.exe'), '--hidden']
    if engine == 'vs8':
        args.append('--vs8')
    subprocess.run(args, check=True, timeout=30, creationflags=NO_WINDOW)
    state = ROOT / 'logs' / ('last-process-vs8.txt' if engine == 'vs8' else 'last-process.txt')
    pid = int(state.read_text().strip())
    marker = {'engine': engine, 'pid': pid, 'createdFileTime': creation_time(pid)}
    marker_path.write_text(json.dumps(marker), encoding='utf-8')
    return marker


class Client:
    def __init__(self, marker):
        self.marker = marker
        self.events = queue.Queue()
        self.counter = 0
        args = [str(BRIDGE), str(marker['pid']), marker['engine'], str(ROOT), str(WORK),
                str(LOGS / (marker['engine'] + '-session.json')), '--stdio']
        self.process = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                        stderr=subprocess.PIPE, text=True, encoding='utf-8',
                                        creationflags=NO_WINDOW, bufsize=1)
        self.transcript = []
        def read():
            for line in self.process.stdout:
                try:
                    item = json.loads(line)
                except ValueError:
                    item = {'invalid_stdout': line}
                self.transcript.append({'response': item})
                self.events.put(item)
            self.events.put(None)
        threading.Thread(target=read, daemon=True).start()
        ready = self.events.get(timeout=20)
        if not ready or ready.get('type') != 'ready':
            self.close()
            raise RuntimeError(json.dumps(ready))

    def request(self, method, params=None, expect_error=None, timeout=40):
        self.counter += 1
        item = {'id': str(self.counter), 'method': method, 'params': params or {}}
        self.transcript.append({'request': item})
        self.process.stdin.write(json.dumps(item, ensure_ascii=False) + '\n')
        self.process.stdin.flush()
        response = self.events.get(timeout=timeout)
        if not response or response.get('id') != item['id']:
            raise RuntimeError('Bridge protocol stream ended or response ID mismatched: ' + repr(response))
        if expect_error:
            assert not response['ok'] and response['error']['code'] == expect_error, response
            return response['error']
        if not response['ok']:
            raise RuntimeError(json.dumps(response))
        return response['result']

    def close(self):
        if self.process.stdin and not self.process.stdin.closed:
            self.process.stdin.close()
        try:
            self.process.wait(timeout=5)
        except subprocess.TimeoutExpired:
            self.process.kill()  # Own disposable bridge helper only; never terminate the IDE here.
            self.process.wait(timeout=5)
        if hasattr(self, 'marker'):
            (LOGS / (self.marker['engine'] + '-transcript.json')).write_text(
                json.dumps(self.transcript, ensure_ascii=False, indent=2), encoding='utf-8')


def run():
    WORK.mkdir(parents=True, exist_ok=True)
    LOGS.mkdir(parents=True, exist_ok=True)
    result = {'schemaVersion': 1, 'observations': {}, 'limitations': []}
    markers = {engine: owned_host(engine) for engine in ['mse', 'vs8']}
    for engine, marker in markers.items():
        print('AUDIT', engine, marker['pid'], flush=True)
        client = None
        try:
            # VS can publish its DTE object after the GUI startup path has returned.
            deadline = time.monotonic() + 25
            while True:
                try:
                    client = Client(marker)
                    break
                except RuntimeError:
                    if time.monotonic() >= deadline:
                        raise
                    time.sleep(1)
            observation = result['observations'][engine] = {'identity': client.request('host.identity')}
            inspection = client.request('host.inspect')
            observation['inspect'] = {k: v for k, v in inspection.items() if k != 'commands'}
            (LOGS / (engine + '-commands.json')).write_text(json.dumps(inspection['commands'], indent=2), encoding='utf-8')

            document = WORK / (engine + '.txt')
            document.write_text('OpenMSE bridge audit\n', encoding='utf-8-sig')
            client.request('documents.open', {'path': str(document)})
            before = client.request('documents.read', {'path': str(document)})
            client.request('documents.write', {'path': str(document), 'expectedText': 'stale', 'text': 'must not overwrite'}, expect_error='conflict')
            content = 'OpenMSE unified kernel bridge\n' + '\u4e2d\u6587\u6587\u6863\u9a8c\u8bc1\n'
            client.request('documents.write', {'path': str(document), 'expectedText': before['text'], 'text': content})
            client.request('documents.close', {'path': str(document)}, expect_error='unsaved_document')
            client.request('documents.save', {'path': str(document)})
            after = client.request('documents.read', {'path': str(document)})
            assert after['saved'] and after['text'].replace('\r\n', '\n') == content
            assert document.read_text(encoding='utf-8-sig').replace('\r\n', '\n') == content
            client.request('documents.close', {'path': str(document)})
            observation['document_roundtrip'] = {'passed': True, 'unicode': True, 'disk_write_verified': True}
            client.request('documents.open', {'path': str(ROOT / 'README.md')}, expect_error='path_outside_workspace')
            client.request('unknown.audit.method', expect_error='method_not_supported')
            client.request('debug.evaluate', {'expression': '1+1'}, expect_error='invalid_state')
            observation['guards'] = {'passed': True, 'checks': ['stale_buffer', 'unsaved_close', 'outside_workspace', 'unknown_method', 'invalid_debug_state']}

            if engine == 'vs8':
                project = WORK / 'DebugSample'
                if not project.exists():
                    shutil.copytree(ROOT / 'samples/VS8Smoke', project, ignore=shutil.ignore_patterns('bin', 'obj', '*.suo', '*.user'))
                source = project / 'Program.cs'
                source.write_text('using System;\ninternal static class Program\n{\n    private static int Main()\n    {\n        int answer = 41;\n        answer = answer + 1;\n        Console.WriteLine(answer);\n        return 0;\n    }\n}\n', encoding='utf-8')
                config = project / 'VS8Smoke.csproj'
                text = config.read_text()
                text = text.replace('<OutputType>Exe</OutputType>', '<OutputType>WinExe</OutputType>')
                if '<PlatformTarget>' not in text:
                    text = text.replace('<OutputType>WinExe</OutputType>', '<OutputType>WinExe</OutputType>\n    <PlatformTarget>x86</PlatformTarget>')
                config.write_text(text, encoding='utf-8')
                observation['project'] = client.request('projects.open', {'path': str(project / 'VS8Smoke.sln')})
                build = client.request('build.run', timeout=60)
                assert build['failedProjects'] == 0, build
                observation['build'] = {'passed': True, 'result': build}
                line = next(i + 1 for i, s in enumerate(source.read_text().splitlines()) if 'answer = answer + 1' in s)
                try:
                    breakpoint = client.request('debug.breakpoints.add', {'path': str(source), 'line': line})
                    assert breakpoint['count'] > 0
                    client.request('debug.start')
                    deadline = time.monotonic() + 25
                    while True:
                        state = client.request('debug.inspect')
                        if state['mode'] == 'dbgBreakMode': break
                        if time.monotonic() >= deadline: raise TimeoutError('Owned debug target did not stop at breakpoint: ' + repr(state))
                        time.sleep(.2)
                    initial = client.request('debug.evaluate', {'expression': 'answer'})
                    client.request('debug.step_over')
                    deadline = time.monotonic() + 10
                    while True:
                        stepped = client.request('debug.inspect')
                        if stepped['mode'] == 'dbgBreakMode': break
                        if time.monotonic() >= deadline: raise TimeoutError('Step operation did not return to break mode')
                        time.sleep(.2)
                    evaluated = client.request('debug.evaluate', {'expression': 'answer'})
                    assert initial['valid'] and initial['value'] == '41', initial
                    assert evaluated['valid'] and evaluated['value'] == '42', evaluated
                    observation['debug'] = {'passed': True, 'breakpoint_line': line, 'before': initial, 'after': evaluated, 'stack': state.get('stack'), 'step_state': stepped}
                except Exception as error:
                    observation['debug'] = {'passed': False, 'error': str(error)}
                finally:
                    state = client.request('debug.inspect')
                    if state['mode'] != 'dbgDesignMode':
                        client.request('debug.stop')
                        for _ in range(30):
                            if client.request('debug.inspect')['mode'] == 'dbgDesignMode': break
                            time.sleep(.1)
        except Exception as error:
            result['observations'].setdefault(engine, {})['error'] = str(error)
        finally:
            if client is not None:
                # Shutdown is deliberately separate from EOF/detach.
                try: result['observations'][engine]['shutdown'] = client.request('host.shutdown')
                except Exception as error: result['observations'][engine]['shutdown_error'] = str(error)
                client.close()
            (LOGS / 'results.json').write_text(json.dumps(result, ensure_ascii=False, indent=2), encoding='utf-8')
        print('RESULT', engine, json.dumps(result['observations'][engine], ensure_ascii=True), flush=True)
    print('Audit report:', LOGS / 'results.json', flush=True)


if __name__ == '__main__':
    run()
