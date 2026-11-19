"""Rebuild the standard toolbox catalog from the imported MSE config and CLR controls."""
from pathlib import Path
import json

root = Path(__file__).resolve().parents[1]
mapping = {
    'Span': 'Label', 'Button': 'Button', 'Reset Button': 'Button',
    'Submit Button': 'Button', 'Text Field': 'TextBox', 'Text Area': 'RichTextBox',
    'File Field': 'TextBox', 'Password Field': 'TextBox', 'Checkbox': 'CheckBox',
    'Radio Button': 'RadioButton', 'Table': 'DataGridView',
    'Linear Layout Panel': 'Panel', 'Grid Layout Panel': 'Panel',
    'Image': 'PictureBox', 'Listbox': 'ListBox', 'Dropdown': 'ComboBox',
    'Horizontal Rule': 'Panel',
}
nodes = json.loads((root / 'config/registry-template.json').read_text(encoding='utf-8-sig'))['Nodes']
entries = []
for node in nodes:
    if '\\DefaultToolbox\\Tab 0\\Element ' not in node['Path']:
        continue
    values = {item['Name']: item['Data'] for item in node['Values']}
    name, markup = values.get('Name', ''), values.get('Data', '')
    if name in mapping and isinstance(markup, str) and markup.startswith('<'):
        entries.append(dict(Name=name, Family='HTML', Markup=markup, ControlType=mapping[name]))
if len(entries) != 17:
    raise RuntimeError('Unexpected MSE toolbox inventory; inspect the imported config first.')
controls = ['Button', 'Label', 'TextBox', 'RichTextBox', 'CheckBox', 'RadioButton',
            'ComboBox', 'ListBox', 'CheckedListBox', 'GroupBox', 'Panel', 'PictureBox',
            'ProgressBar', 'TrackBar', 'NumericUpDown', 'DateTimePicker', 'MonthCalendar',
            'TreeView', 'ListView', 'TabControl', 'LinkLabel', 'DataGridView']
entries.extend(dict(Name=name, Family='Windows Forms', Markup=None, ControlType=name) for name in controls)
(root / 'config/toolbox.json').write_text(json.dumps(entries, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
print('Exported 17 MSE HTML entries and 22 standard Windows Forms entries.')
