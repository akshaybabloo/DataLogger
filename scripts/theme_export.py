import os
from shutil import rmtree
from pathlib import Path

from jinja2 import Environment, FileSystemLoader, select_autoescape
from qt_material import export_theme

root_path = os.path.abspath('../')

theme_path = os.path.join(root_path, "theme")
qss_path = os.path.join(root_path, "style.qss")
qrc_path = os.path.join(root_path, "style.qrc")

if Path(theme_path).is_dir():
    rmtree(theme_path)

export_theme(theme='dark_blue.xml', qss=qss_path, output=theme_path, prefix=':theme/')

p = Path(theme_path)
files = ['/'.join(a.parts[-3:]) for a in p.glob("**/*.svg")]

env = Environment(loader=FileSystemLoader("."), autoescape=select_autoescape(['html', 'xml']))
template = env.get_template("styles.qrc")
render = template.render(files=files)

with open(qrc_path, 'w') as f:
    f.write(render)
