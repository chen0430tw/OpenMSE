"""Import the installed VS2005 desktop IDE into the existing OpenMSE project."""
import base64
import hashlib
import json
from pathlib import Path
import shutil
import winreg as w

ROOT = Path(r'D:\OpenMSE')
SOURCE = Path(r'C:\Program Files (x86)\Microsoft Visual Studio 8')
DEST = ROOT / 'runtime'
assert ROOT.is_dir() and not (DEST / 'Common7/IDE/devenv.exe').exists(), 'VS8 engine must not already be imported'
assert (SOURCE / 'Common7/IDE/devenv.exe').is_file()
DEST.mkdir(parents=True, exist_ok=True)
manifest = []
for name in [folder.name for folder in SOURCE.iterdir() if folder.is_dir()]:
    folder = SOURCE / name
    for src in folder.rglob('*'):
        if not src.is_file():
            continue
        relative = src.relative_to(SOURCE)
        target = DEST / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        if target.exists() and hashlib.sha256(target.read_bytes()).digest() != hashlib.sha256(src.read_bytes()).digest():
            raise RuntimeError("Conflicting runtime file: " + str(target))
        if not target.exists(): shutil.copy2(src, target)
        digest = hashlib.sha256(src.read_bytes()).hexdigest()
        assert hashlib.sha256(target.read_bytes()).hexdigest() == digest
        manifest.append({'source': str(src), 'destination': str(target.relative_to(ROOT)),
                         'size': src.stat().st_size, 'sha256': digest})
    print('Copied', name, 'total files', len(manifest), flush=True)

for src in SOURCE.iterdir():
    if src.is_file():
        target = DEST / src.name
        if target.exists() and hashlib.sha256(target.read_bytes()).digest() != hashlib.sha256(src.read_bytes()).digest():
            raise RuntimeError("Conflicting runtime file: " + str(target))
        if not target.exists(): shutil.copy2(src, target)
        digest = hashlib.sha256(src.read_bytes()).hexdigest()
        assert hashlib.sha256(target.read_bytes()).hexdigest() == digest
        manifest.append({'source': str(src), 'destination': str(target.relative_to(ROOT)),
                         'size': src.stat().st_size, 'sha256': digest})

nodes = []
def snapshot(key, relative='', store='MachineVS8'):
    values = []
    count = w.QueryInfoKey(key)
    for index in range(count[1]):
        name, data, kind = w.EnumValue(key, index)
        if name.lower() in {'registeredname', 'registeredorganization', 'registeredpid'}:
            continue
        if store == 'UserVS8' and any(word in name.lower() for word in ['lastloadedsolution', 'mru', 'password', 'token']):
            continue
        if kind not in (1, 2, 3, 4, 7, 11):
            continue
        if isinstance(data, bytes): data = base64.b64encode(data).decode('ascii')
        if store == 'UserVS8' and isinstance(data, str) and data.lower().endswith('.vssettings'):
            source_settings = Path(data)
            if source_settings.is_file():
                target_settings = ROOT / 'workspace/VS8/Settings' / source_settings.name
                target_settings.parent.mkdir(parents=True, exist_ok=True)
                if not target_settings.exists(): shutil.copy2(source_settings, target_settings)
                data = '@OPENMSE@\\workspace\\VS8\\Settings\\' + source_settings.name
        values.append({'Name': name, 'Type': kind, 'Data': data})
    nodes.append({'Store': store, 'Path': relative, 'Values': values})
    for index in range(count[0]):
        child = w.EnumKey(key, index)
        if not relative and child.lower() in ['licenses', 'registration', 'filemrulist', 'projectmrulist']:
            continue
        if store == 'UserVS8' and not relative and child not in ['General', 'Profile', 'FontAndColors', 'Keyboard', 'LanguageResources', 'Languages', 'Text Editor']:
            continue
        with w.OpenKey(key, child) as sub:
            snapshot(sub, relative + ('\\' if relative else '') + child, store)

for hive, store in [(w.HKEY_LOCAL_MACHINE, 'MachineVS8'), (w.HKEY_CURRENT_USER, 'UserVS8')]:
    with w.OpenKey(hive, r'SOFTWARE\Microsoft\VisualStudio\8.0', 0, w.KEY_READ | w.KEY_WOW64_32KEY) as key:
        snapshot(key, store=store)
for node in nodes:
    if node['Store'] == 'UserVS8' and not node['Path']:
        for v in node['Values']:
            if any(part in v['Name'] for part in ['Location', 'DefaultFile', 'DefaultOpen', 'DefaultNew']):
                if v['Name'] == 'VisualStudioLocation':
                    v['Data'] = '@OPENMSE@\\runtime'
                elif v['Name'] != 'MyDocumentsLocation':
                    v['Data'] = '@OPENMSE@\\workspace\\VS8'
        node['Values'].append({'Name': 'LastLoadedSolution', 'Type': 2, 'Data': ''})
(ROOT / 'workspace/VS8').mkdir(parents=True, exist_ok=True)
(ROOT / 'config/vs8-registry-template.json').write_text(json.dumps({
    'Mappings': [{'Source': str(SOURCE), 'Target': r'runtime'},
                 {'Source': '@OPENMSE@', 'Target': '.'}],
    'Nodes': nodes}, ensure_ascii=False, indent=2), encoding='utf-8')
(ROOT / 'config/vs8-files.json').write_text(json.dumps(manifest, indent=2), encoding='utf-8')
print('VS8 imported:', len(manifest), 'files;', round(sum(x['size'] for x in manifest)/1024**2,1), 'MiB;', len(nodes), 'registry nodes', flush=True)
