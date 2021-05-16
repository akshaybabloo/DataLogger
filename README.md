# DataLogger

DataLogger is a Bluetooth LE streamer and an HTTP server.

Table of Contents

- [Instillation](#instillation)
- [Usage](#usage)

## Instillation

This code depends on Qt 5.15 and this can be downloaded from [Qt](https://qt.io).

From the maintenance tool, make sure you have the following selected (under Qt 5.15.*):

1. macOS (only in mac)
2. MSVC 2019 64-bit (only in windows)
3. Sources
4. Qt Charts
5. Qt Debug Information files

And under `Developer and Designer Tools`, select:

1. Qt Creator \<version\> Debug Symbols
2. Qt Installer framework \<version\> (this is only required if you want to distribute the software)
3. CMake \<version\>
4. Ninja \<version\>

> Note: `version` means, whatever the current version is.

> Warning: this application uses OpenGL, make sure you have a compatible hardware for it.

## Usage

1. Open Qt Creator (the IDE that comes with Qt), then `File -> Open File or Project...` then selection `DataLogger/` folder. 
2. The IDE should be able to automatically detect `CMakeLists.txt` file and open a configuration view, make sure you select the a configuration that looks like - `Desktop Qt 5.15.* <build type>`
3. Clicking on the play button (bottom left) should run the program and open a GUI.