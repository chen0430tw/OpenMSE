"""Populate a new local OpenMSE workspace from the inspected installation."""
import base64
import hashlib
import json
from pathlib import Path
import shutil
import winreg

ROOT = Path(r'D:\OpenMSE')
STAGE = Path(r'C:\Users\asus\OpenMSE')
OFFICE = Path(r'C:\Program Files (x86)\Common Files\Microsoft Shared\OFFICE12')
DEBUG = Path(r'C:\Program Files (x86)\Common Files\Microsoft Shared\VS7Debug')
assert not ROOT.exists(), 'Target must be a new project directory'
shutil.copytree(STAGE, ROOT)
for folder in ['runtime/OFFICE12', 'runtime/VS7Debug', 'config', 'logs', 'build', 'vendor']:
    (ROOT / folder).mkdir(parents=True, exist_ok=True)

manifest = []
def copy(source, target):
    assert source.is_file(), source
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, target)
    assert hashlib.sha256(source.read_bytes()).digest() == hashlib.sha256(target.read_bytes()).digest()
    manifest.append({'source': str(source), 'destination': str(target.relative_to(ROOT)),
                     'size': source.stat().st_size,
                     'sha256': hashlib.sha256(target.read_bytes()).hexdigest()})

names = ['MSE7.EXE', 'MSO.DLL', 'MSOMSE.DLL', 'MSORES.DLL', 'MODHELP.DLL',
         'OGL.DLL', 'RICHED20.DLL', 'MSPTLS.DLL', 'MSOXEV.DLL', 'MSOXMLMF.DLL',
         'VIEWSSPT.XML', 'VIEWSSRC.XML', 'VISUALUI.TTF', 'MSOICONS.EXE']
for name in names:
    copy(OFFICE / name, ROOT / 'runtime/OFFICE12' / name)
for source in (OFFICE / 'VS Runtime').rglob('*'):
    if source.is_file(): copy(source, ROOT / 'runtime/OFFICE12/VS Runtime' / source.relative_to(OFFICE / 'VS Runtime'))
for name in ['MSEINTL.DLL', 'MSOINTL.DLL', 'MODHLPUI.DLL', 'ALRTINTL.DLL']:
    source = OFFICE / '2052' / name
    if source.is_file(): copy(source, ROOT / 'runtime/OFFICE12/2052' / name)
for source in DEBUG.rglob('*'):
    if source.is_file(): copy(source, ROOT / 'runtime/VS7Debug' / source.relative_to(DEBUG))

nodes = []
def snapshot(key, relative=''):
    values = []
    count = winreg.QueryInfoKey(key)
    for index in range(count[1]):
        name, value, kind = winreg.EnumValue(key, index)
        if isinstance(value, bytes): value = base64.b64encode(value).decode('ascii')
        values.append({'Name': name, 'Type': kind, 'Data': value})
    nodes.append({'Path': relative, 'Values': values})
    for index in range(count[0]):
        child = winreg.EnumKey(key, index)
        with winreg.OpenKey(key, child) as handle:
            snapshot(handle, relative + ('\\' if relative else '') + child)

with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r'SOFTWARE\Microsoft\Office\12.0\MSE',
                    0, winreg.KEY_READ | winreg.KEY_WOW64_32KEY) as handle:
    snapshot(handle)
(ROOT / 'config/registry-template.json').write_text(json.dumps({
    'Mappings': [{'Source': str(OFFICE), 'Target': r'runtime\OFFICE12'},
                 {'Source': str(DEBUG), 'Target': r'runtime\VS7Debug'}],
    'Nodes': nodes}, ensure_ascii=False, indent=2), encoding='utf-8')
(ROOT / 'config/files.json').write_text(json.dumps(manifest, indent=2), encoding='utf-8')
print('Copied', len(manifest), 'files;', sum(x['size'] for x in manifest), 'bytes;', len(nodes), 'registry nodes')
print('Original files and original registry keys were not modified.')
