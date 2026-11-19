"""OpenMSE packaging (integration plan phase 6).

  python scripts/package.py trace      Run the legacy scenarios with file tracing; writes logs/package-trace/*.txt
  python scripts/package.py manifest   Classify the traces into config/packages/*.json (closures and external dependencies)
  python scripts/package.py assemble OUT PACKAGE [PACKAGE ...]
                                       Copy the listed packages into OUT (a new directory) and write OUT/package-manifest.json
  python scripts/package.py scenario-vs8-build ROOT WORK
  python scripts/package.py scenario-mse-window ROOT
                                       Scenario drivers, also used to verify an assembled package from its own root.

Packages:
  core          unified GUI, kernel, CLR 2.0 debug adapters, toolbox, samples, docs; no legacy runtime
  vs8-backend   VS2005 back-end host: measured runtime closure + launcher/hook/bridge/registry template
  mse-window    MSE compatibility window: measured runtime closure + launcher/hook/registry template
  legacy-full   the complete runtime/ directory (every legacy scenario, including unmeasured ones)
Only scenarios listed in config/packages.json are claimed for a package; anything else needs legacy-full.
"""
from pathlib import Path
import ctypes
from ctypes import wintypes
import hashlib
import json
import os
import queue
import shutil
import subprocess
import sys
import threading
import time

ROOT = Path(__file__).resolve().parents[1]
TRACE_DIR = ROOT / 'logs' / 'package-trace'
NO_WINDOW = subprocess.CREATE_NO_WINDOW
kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)
user32 = ctypes.WinDLL('user32', use_last_error=True)


def creation_time(pid):
    kernel32.OpenProcess.restype = wintypes.HANDLE
    handle = kernel32.OpenProcess(0x1000, False, pid)
    if not handle:
        raise ProcessLookupError(pid)
    times = [wintypes.FILETIME() for _ in range(4)]
    try:
        kernel32.GetProcessTimes(wintypes.HANDLE(handle), *(ctypes.byref(t) for t in times))
        return str((times[0].dwHighDateTime << 32) | times[0].dwLowDateTime)
    finally:
        kernel32.CloseHandle(wintypes.HANDLE(handle))


def end_owned(pid, created):
    """Terminate a legacy instance this script started, after re-checking its identity."""
    try:
        if creation_time(pid) == created:
            subprocess.run(['taskkill', '/PID', str(pid), '/F'], capture_output=True)
    except ProcessLookupError:
        pass


def launch(root, engine, extra, env):
    args = [str(root / 'OpenMSE.exe'), '--' + engine] + extra
    started = time.time()
    subprocess.run(args, cwd=root, env=env, timeout=120, creationflags=NO_WINDOW, check=True)
    state = root / 'logs' / ('last-process-vs8.txt' if engine == 'vs8' else 'last-process.txt')
    pid = int(state.read_text().split()[0])
    created = creation_time(pid)
    if state.stat().st_mtime < started - 2:
        raise RuntimeError('launcher did not write a new process id')
    return pid, created


class Bridge:
    def __init__(self, root, pid, created, work):
        marker = Path(work) / 'bridge-marker.json'
        marker.write_text(json.dumps({'engine': 'vs8', 'pid': pid, 'createdFileTime': created}), encoding='utf-8')
        deadline = time.time() + 90
        while True:
            self.p = subprocess.Popen([str(root / 'build' / 'HostBridge.exe'), str(pid), 'vs8', str(root), str(work), str(marker), '--stdio'],
                                      stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                      text=True, encoding='utf-8', creationflags=NO_WINDOW, bufsize=1)
            first = self.p.stdout.readline()
            if '"ready"' in first:
                break
            self.p.wait()
            if time.time() > deadline:
                raise RuntimeError('bridge could not attach: ' + first)
            time.sleep(1)
        self.q = queue.Queue()
        threading.Thread(target=lambda: [self.q.put(json.loads(l)) for l in self.p.stdout], daemon=True).start()
        self.n = 0

    def call(self, method, params=None, timeout=120):
        self.n += 1
        self.p.stdin.write(json.dumps({'id': str(self.n), 'method': method, 'params': params or {}}, ensure_ascii=False) + '\n')
        self.p.stdin.flush()
        reply = self.q.get(timeout=timeout)
        if not reply.get('ok'):
            raise RuntimeError(method + ': ' + json.dumps(reply, ensure_ascii=False))
        return reply['result']

    def close(self):
        try:
            self.p.stdin.close()
            self.p.wait(10)
        except Exception:
            self.p.kill()


def scenario_vs8_build(root, work, env=None):
    """Owned hidden VS2005: open, edit and save a document; open a solution; build it through DTE; shut down."""
    root, work = Path(root), Path(work)
    work.mkdir(parents=True, exist_ok=True)
    project = work / 'Vs8Build'
    if project.exists():
        shutil.rmtree(project)
    shutil.copytree(root / 'samples' / 'VS8Smoke', project, ignore=shutil.ignore_patterns('bin', 'obj', '*.suo', '*.user'))
    pid, created = launch(root, 'vs8', ['--hidden'], env or os.environ.copy())
    try:
        bridge = Bridge(root, pid, created, work)
        try:
            doc = project / 'notes.txt'
            doc.write_text('OpenMSE package scenario\n', encoding='utf-8-sig')
            bridge.call('documents.open', {'path': str(doc)})
            before = bridge.call('documents.read', {'path': str(doc)})
            bridge.call('documents.write', {'path': str(doc), 'expectedText': before['text'], 'text': 'OpenMSE 包场景\n'})
            bridge.call('documents.save', {'path': str(doc)})
            assert doc.read_text(encoding='utf-8-sig').replace('\r\n', '\n') == 'OpenMSE 包场景\n'
            bridge.call('documents.close', {'path': str(doc)})
            opened = bridge.call('projects.open', {'path': str(project / 'VS8Smoke.sln')})
            built = bridge.call('build.run', timeout=240)
            exe = next(project.glob('bin/Debug/*.exe'), None)
            result = {'projects': opened['projects'], 'failedProjects': built['failedProjects'], 'artifact': str(exe) if exe else None}
            assert built['failedProjects'] == 0 and exe is not None, result
            out = subprocess.run([str(exe)], capture_output=True, text=True, timeout=30, creationflags=NO_WINDOW)
            result['exitCode'] = out.returncode
            bridge.call('host.shutdown', timeout=60)
            for _ in range(300):
                try:
                    creation_time(pid)
                except ProcessLookupError:
                    break
                time.sleep(0.1)
            return result
        finally:
            bridge.close()
    finally:
        end_owned(pid, created)


def dialogs(pid):
    found = []
    proc = ctypes.WINFUNCTYPE(wintypes.BOOL, wintypes.HWND, wintypes.LPARAM)
    def top(h, _):
        owner = wintypes.DWORD()
        user32.GetWindowThreadProcessId(h, ctypes.byref(owner))
        cls = ctypes.create_unicode_buffer(64)
        user32.GetClassNameW(h, cls, 64)
        if owner.value == pid and cls.value in ('#32770', 'wndclass_desked_gsk') and user32.IsWindowVisible(h):
            title = ctypes.create_unicode_buffer(512)
            user32.GetWindowTextW(h, title, 512)
            found.append((cls.value, title.value))
        return True
    user32.EnumWindows(proc(top), 0)
    return found


def scenario_mse_window(root, env=None):
    """MSE compatibility window: start MSE with samples/hello.htm and verify modules, design window and no error dialog."""
    root = Path(root)
    pid, created = launch(root, 'mse', ['--hidden', str(root / 'samples' / 'hello.htm')], env or os.environ.copy())
    try:
        report = None
        for _ in range(60):
            time.sleep(1)
            check = subprocess.run(['powershell.exe', '-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', str(root / 'scripts' / 'verify.ps1'),
                                    '-ProcessId', str(pid), '-Engine', 'mse'], capture_output=True, text=True, creationflags=NO_WINDOW)
            if check.returncode == 0:
                report = json.loads((root / 'logs' / 'validation.json').read_text(encoding='utf-8-sig'))
                break
        if report is None:
            raise RuntimeError('MSE did not reach a verified design window; windows: ' + repr(dialogs(pid)))
        return {'passed': report['Passed'], 'modules': report['ModuleCount'], 'design': [w['Title'] for w in report['DesignWindows']],
                'originalPrivateModules': len(report['OriginalPrivateModules'])}
    finally:
        end_owned(pid, created)


def trace():
    TRACE_DIR.mkdir(parents=True, exist_ok=True)
    for old in TRACE_DIR.glob('*.txt'):
        old.unlink()
    env = os.environ.copy()
    work = ROOT / 'workspace' / 'package-trace'
    if work.exists():
        shutil.rmtree(work)
    work.mkdir(parents=True)
    results = {}
    env['OPENMSE_TRACE_FILES'] = str(TRACE_DIR / 'vs8-build-{engine}.txt')
    results['vs8-build'] = scenario_vs8_build(ROOT, work / 'build', env)
    print('vs8-build', results['vs8-build'], flush=True)
    env['OPENMSE_TRACE_FILES'] = str(TRACE_DIR / 'host-tests-{engine}.txt')
    host = subprocess.run([str(ROOT / 'build' / 'host-tests.exe'), str(ROOT), str(work / 'hosts')], env=env, capture_output=True,
                          text=True, encoding='utf-8', errors='replace', timeout=900)
    results['host-tests'] = host.stdout.strip().splitlines()[-1] if host.stdout.strip() else host.stderr
    print('host-tests', results['host-tests'], flush=True)
    if host.returncode != 0:
        raise RuntimeError('host-tests failed during tracing:\n' + host.stdout[-3000:])
    env['OPENMSE_TRACE_FILES'] = str(TRACE_DIR / 'mse-window-{engine}.txt')
    results['mse-window'] = scenario_mse_window(ROOT, env)
    print('mse-window', results['mse-window'], flush=True)
    (TRACE_DIR / 'results.json').write_text(json.dumps(results, ensure_ascii=False, indent=2), encoding='utf-8')


def tracked_runtime():
    out = subprocess.run(['git', '-C', str(ROOT), 'ls-files', '-z', 'runtime'], capture_output=True, check=True).stdout.decode('utf-8')
    files = {}
    for rel in filter(None, out.split('\0')):
        files[rel.replace('/', '\\').lower()] = rel.replace('/', '\\')
    return files


def sha256(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(1 << 20), b''):
            h.update(chunk)
    return h.hexdigest().upper()


def classify(trace_files, tracked):
    root = str(ROOT).lower() + '\\'
    windows = os.environ.get('WINDIR', r'C:\Windows').lower() + '\\'
    runtime, directories, external = set(), set(), {'originalInstall': set(), 'gac': set(), 'framework': set()}
    originals = [r'c:\program files (x86)\microsoft visual studio 8\\', r'c:\program files (x86)\common files\microsoft shared\vs7debug\\',
                 r'c:\program files (x86)\common files\microsoft shared\office12\\']
    tracked_dirs = {}
    for key, rel in tracked.items():
        parts = rel.split('\\')
        for i in range(1, len(parts)):
            tracked_dirs['\\'.join(parts[:i]).lower()] = '\\'.join(parts[:i])
    for trace_file in trace_files:
        for line in trace_file.read_text(encoding='utf-8').splitlines():
            path = line.strip()
            low = path.lower()
            if low.startswith(root + 'runtime\\') or low == root + 'runtime':
                rel = low[len(root):].rstrip('\\')
                if rel in tracked:
                    runtime.add(tracked[rel])
                elif rel in tracked_dirs:
                    directories.add(tracked_dirs[rel])
            elif any(low.startswith(o.replace('\\\\', '\\')) for o in originals):
                external['originalInstall'].add(path)
            elif low.startswith(windows + 'assembly\\') and os.path.isfile(path):
                external['gac'].add(path)
            elif low.startswith(windows + 'microsoft.net\\') and os.path.isfile(path):
                external['framework'].add(path)
    return runtime, directories, {k: sorted(v, key=str.lower) for k, v in external.items()}


def pe_imports(path):
    """Names of DLLs in a PE file's import and delay-import tables (empty for non-PE files)."""
    import struct
    try:
        data = Path(path).read_bytes()
    except OSError:
        return []
    if data[:2] != b'MZ' or len(data) < 0x40:
        return []
    pe = struct.unpack_from('<I', data, 0x3C)[0]
    if data[pe:pe + 4] != b'PE\0\0':
        return []
    sections_count = struct.unpack_from('<H', data, pe + 6)[0]
    optional_size = struct.unpack_from('<H', data, pe + 20)[0]
    optional = pe + 24
    magic = struct.unpack_from('<H', data, optional)[0]
    directories = optional + (96 if magic == 0x10B else 112)
    sections = []
    for i in range(sections_count):
        s = optional + optional_size + i * 40
        vsize, va, rsize, raw = struct.unpack_from('<IIII', data, s + 8)
        sections.append((va, max(vsize, rsize), raw))
    def offset(rva):
        for va, size, raw in sections:
            if va <= rva < va + size:
                return rva - va + raw
        return None
    def cstr(rva):
        o = offset(rva)
        if o is None:
            return None
        end = data.find(b'\0', o)
        return data[o:end].decode('ascii', 'replace')
    names = []
    for index, size in ((1, 20), (13, 32)):  # import directory, delay-import directory
        rva = struct.unpack_from('<I', data, directories + index * 8)[0]
        o = offset(rva) if rva else None
        while o is not None and o + size <= len(data):
            name_rva = struct.unpack_from('<I', data, o + (12 if index == 1 else 4))[0]
            if not name_rva:
                break
            name = cstr(name_rva)
            if name:
                names.append(name.lower())
            o += size
    return names


def static_closure(runtime, tracked, entries):
    """Add DLLs reached through static/delay imports: the loader resolves them before the trace hook is active."""
    by_name = {}
    for low, rel in tracked.items():
        if low.endswith(('.dll', '.exe', '.ocx')):
            by_name.setdefault(low.rsplit('\\', 1)[-1], []).append(rel)
    pending = list(runtime) + list(entries)
    seen = set(r.lower() for r in pending)
    added = set()
    while pending:
        rel = pending.pop()
        folder = rel.rsplit('\\', 1)[0].lower()
        for name in pe_imports(ROOT / rel):
            candidates = by_name.get(name, [])
            # The loader looks in the image's own folder first; the IDE folder is also on its DLL path.
            pick = next((c for c in candidates if c.lower().rsplit('\\', 1)[0] == folder), None) or \
                   next((c for c in candidates if c.lower().startswith('runtime\\common7\\ide\\') and c.count('\\') == 3), None) or \
                   next((c for c in candidates if c.count('\\') == 1), None)
            if pick and pick.lower() not in seen:
                seen.add(pick.lower())
                added.add(pick)
                pending.append(pick)
    return added


def manifest():
    packages = json.loads((ROOT / 'config' / 'packages.json').read_text(encoding='utf-8'))
    tracked = tracked_runtime()
    out_dir = ROOT / 'config' / 'packages'
    out_dir.mkdir(exist_ok=True)
    for name, spec in packages['packages'].items():
        if 'traces' not in spec:
            continue
        traces = [p for pattern in spec['traces'] for p in TRACE_DIR.glob(pattern)]
        if not traces:
            raise RuntimeError('no traces for ' + name + '; run: python scripts/package.py trace')
        runtime, directories, external = classify(traces, tracked)
        entries = [tracked[e.replace('/', '\\').lower()] for e in spec.get('entries', []) if e.replace('/', '\\').lower() in tracked]
        imported = static_closure(runtime | set(entries), tracked, [])
        runtime |= set(entries) | imported
        print(name, 'static imports added:', sorted(imported, key=str.lower))
        files = []
        for rel in sorted(runtime, key=str.lower):
            path = ROOT / rel
            files.append({'path': rel.replace('\\', '/'), 'size': path.stat().st_size, 'sha256': sha256(path)})
        data = {'schemaVersion': 1, 'package': name, 'measured': time.strftime('%Y-%m-%d'), 'scenarios': spec['scenarios'],
                'runtimeFiles': files, 'runtimeDirectories': sorted({d.replace('\\', '/') for d in directories}, key=str.lower),
                'externalDependencies': external,
                'totals': {'files': len(files), 'bytes': sum(f['size'] for f in files)}}
        (out_dir / (name + '.json')).write_text(json.dumps(data, ensure_ascii=False, indent=1) + '\n', encoding='utf-8')
        print(name, data['totals'], {k: len(v) for k, v in external.items()})


def assemble(out, names):
    out = Path(out).resolve()
    if out.exists():
        raise SystemExit('target exists; choose a new directory: ' + str(out))
    packages = json.loads((ROOT / 'config' / 'packages.json').read_text(encoding='utf-8'))['packages']
    copied = {}
    def copy(rel):
        src = ROOT / rel
        if not src.is_file():
            raise SystemExit('missing build output or file: ' + rel + ' (build first)')
        dst = out / rel
        dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(src, dst)
        copied[rel.replace('\\', '/')] = {'size': src.stat().st_size, 'sha256': sha256(src)}
    # Wildcards only match files under version control (no user files such as .suo); explicit entries may be build outputs.
    listed = subprocess.run(['git', '-C', str(ROOT), 'ls-files', '-z'], capture_output=True, check=True).stdout.decode('utf-8')
    versioned = {f.replace('/', '\\').lower() for f in listed.split('\0') if f}
    for name in names:
        spec = packages[name]
        for pattern in spec.get('files', []):
            if any(c in pattern for c in '*?'):
                matches = [p for p in ROOT.glob(pattern) if p.is_file() and str(p.relative_to(ROOT)).lower() in versioned]
            else:
                matches = [ROOT / pattern]
            for m in matches:
                copy(str(m.relative_to(ROOT)))
        if spec.get('runtime') == 'all':
            for rel in tracked_runtime().values():
                copy(rel)
        elif 'traces' in spec:
            measured = json.loads((ROOT / 'config' / 'packages' / (name + '.json')).read_text(encoding='utf-8'))
            for f in measured['runtimeFiles']:
                copy(f['path'])
            for d in measured['runtimeDirectories']:
                (out / d).mkdir(parents=True, exist_ok=True)
    (out / 'logs').mkdir(exist_ok=True)
    data = {'schemaVersion': 1, 'packages': names, 'assembled': time.strftime('%Y-%m-%d %H:%M'), 'files': copied}
    (out / 'package-manifest.json').write_text(json.dumps(data, ensure_ascii=False, indent=1) + '\n', encoding='utf-8')
    print('assembled', out, len(copied), 'files', sum(v['size'] for v in copied.values()) // (1024 * 1024), 'MiB')


if __name__ == '__main__':
    command = sys.argv[1] if len(sys.argv) > 1 else ''
    if command == 'trace':
        trace()
    elif command == 'manifest':
        manifest()
    elif command == 'assemble' and len(sys.argv) >= 4:
        assemble(sys.argv[2], sys.argv[3:])
    elif command == 'scenario-vs8-build' and len(sys.argv) == 4:
        print(json.dumps(scenario_vs8_build(sys.argv[2], sys.argv[3]), ensure_ascii=False))
    elif command == 'scenario-mse-window' and len(sys.argv) == 3:
        print(json.dumps(scenario_mse_window(sys.argv[2]), ensure_ascii=False))
    else:
        print(__doc__)
        sys.exit(2)
