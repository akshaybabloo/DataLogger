# DataLogger

DataLogger is a Bluetooth LE streamer and an HTTP server.

Table of Contents

- [1. Requirements](#1-requirements)
- [2. Using conan Package Manager](#2-using-conan-package-manager)
- [3. Instillation](#3-instillation)
- [4. Usage](#4-usage)

## 1. Requirements

1. Qt Framework - download it from [Qt](https://qt.io)
2. conan - package manager - `pip install conan` (Python 3.6+ is needed)
3. Xcode (only on mac)
4. Visual Studio 2019 with C++ (only on windows)

## 2. Using conan Package Manager

This project depends on the following external library and they need to be installed first using `conan` from terminal

| Package Name | Instillation Instruction   |
|--------------|----------------------------|
| Eigen 3.3.9  | conan install eigen/3.3.9@ |

These packages are predefined in `conanfile.txt`, before running the application do - `conan install .`

## 3. Instillation

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

> Note: `version` means, whatever the current version is in use.

> Warning: this application uses OpenGL, make sure you have a compatible hardware for it.

## 4. Usage

1. Open Qt Creator (the IDE that comes with Qt), `File -> Open File or Project...` then select `DataLogger/` folder. 
2. The IDE should be able to automatically detect `CMakeLists.txt` file and open a configuration view, make sure you select the a configuration that looks like - `Desktop Qt 5.15.* <build type>` - `<build type>` could be `clang 64bit` (mac) or `msvc 64-bit` (windows).
3. Clicking on the play button (bottom left) should run the program and open a GUI.