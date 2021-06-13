cmake_minimum_required(VERSION 3.5)

project(DataLogger VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(conan_paths.cmake)

# CLion setting. This will not work in Windows.
if ($ENV{CLION_IDE})
    set(QT_VERSION_MAJOR 5)
    # Note: This path needs to be update on every new Qt version
    if (WIN32)
        # For some reason its not working on Windows. Use Qt Creator to make it runnable.
        set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64")
    elseif (APPLE)
        set(CMAKE_PREFIX_PATH /Users/akshayrajgollahalli/Qt/5.15.2/clang_64)
    endif ()
endif ()

if (APPLE)
    set(MACOSX_BUNDLE_ICON_FILE datalogger.icns)
    # And the following tells CMake where to find and install the file itself.
    set(app_icon_macos "resources/images/datalogger.icns")
    set_source_files_properties(${app_icon_macos} PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources")
endif ()

if (WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/win.rc")
endif ()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Bluetooth Network Charts Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Bluetooth REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Charts REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)
find_package(Eigen3 REQUIRED)

set(PROJECT_SOURCES
        MACOSX_BUNDLE

        main.cpp

        resources.qrc
        style.qrc

        ${app_icon_macos}
        ${APP_ICON_RESOURCE_WINDOWS}

        mainwindow.cpp mainwindow.h mainwindow.ui
        logger.cpp logger.h logger.ui

        widgets/listwidget.cpp widgets/listwidget.h widgets/listwidget.ui
        widgets/settingswidget.cpp widgets/settingswidget.h widgets/settingswidget.ui
        widgets/statusbarindicator.cpp widgets/statusbarindicator.h widgets/statusbarindicator.ui
        widgets/aboutwidget.cpp widgets/aboutwidget.h widgets/aboutwidget.ui
        widgets/channelwidget.h widgets/channelwidget.cpp widgets/channelwidget.ui

        utils.cpp utils.h
        deviceinfo.cpp deviceinfo.h
        serviceinfo.cpp serviceinfo.h chart.cpp chart.h)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DataLogger
            ${PROJECT_SOURCES}
            )
else ()
    add_executable(DataLogger
            ${PROJECT_SOURCES}
            )
endif ()

set_property(TARGET DataLogger PROPERTY MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")

target_link_libraries(DataLogger PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Charts
        Qt${QT_VERSION_MAJOR}::Bluetooth
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Gui
        Eigen3::Eigen
        )

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DataLogger)
endif ()
