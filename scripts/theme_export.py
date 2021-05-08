from qt_material import export_theme
import os

theme_path = os.path.join(os.path.abspath('../'), "theme")
qss_path = os.path.join(os.path.abspath('../'), "style.qss")
export_theme(theme='dark_teal.xml', qss=qss_path, output=theme_path, prefix='icon:/theme/')
