import os
from pathlib import Path
from shutil import rmtree

from jinja2 import Environment, FileSystemLoader, select_autoescape
from qt_material import export_theme

root_path = os.path.abspath('../')

theme_path = os.path.join(root_path, "theme")
dark_theme_path = os.path.join(theme_path, "dark")
light_theme_path = os.path.join(theme_path, "light")

qss_path_dark = os.path.join(root_path, "style-dark.qss")
qss_path_light = os.path.join(root_path, "style-light.qss")

qrc_path = os.path.join(root_path, "style.qrc")

if Path(theme_path).is_dir():
    rmtree(theme_path)

export_theme(theme='dark_blue.xml', qss=qss_path_dark, output=dark_theme_path, prefix=':theme/dark/')
export_theme(theme='light_blue.xml', qss=qss_path_light, output=light_theme_path, prefix=':theme/light/')

p = Path(theme_path)
files = ['/'.join(a.parts[-4:]) for a in p.glob("**/*.svg")]

env = Environment(loader=FileSystemLoader("."), autoescape=select_autoescape(['html', 'xml']))
template = env.get_template("styles.qrc")
render = template.render(files=files)

with open(qrc_path, 'w') as f:
    f.write(render)
