cmake_minimum_required(VERSION 3.5)

project(DataLogger VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(conan_paths.cmake)

# CLion setting. This will not work in Windows.
if ($ENV{CLION_IDE})
    set(QT_VERSION_MAJOR 6)
    # Note: This path needs to be update on every new Qt version
    if (WIN32)
        # For some reason its not working on Windows. Use Qt Creator to make it runnable.
        set(CMAKE_PREFIX_PATH "C:/Qt/6.2.4/msvc2019_64")
    elseif (APPLE)
        set(CMAKE_PREFIX_PATH /Users/akshayrajgollahalli/Qt/6.2.4/macos)
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

find_package(QT NAMES Qt6 COMPONENTS Widgets Bluetooth Network Charts Gui REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Bluetooth REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Qt6 COMPONENTS Charts REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS Concurrent REQUIRED)
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

qt_add_executable(DataLogger
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        )

if (APPLE)
    set_property(TARGET DataLogger PROPERTY MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")
endif ()

set_target_properties(DataLogger PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER datalogger.gollahalli.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        )

target_link_libraries(DataLogger PRIVATE
        Qt6::Widgets
        Qt6::Charts
        Qt6::Bluetooth
        Qt6::Network
        Qt6::Gui
        Qt6::Concurrent
        Eigen3::Eigen
        )

qt_finalize_executable(DataLogger)
