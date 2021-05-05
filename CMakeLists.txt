cmake_minimum_required(VERSION 3.5)

project(DataLogger LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ($ENV{CLION_IDE})
    if(WIN32)
        # Note: This path needs to be update on every new Qt version
        set(CMAKE_PREFIX_PATH C:/Qt/5.15.2/msvc2019_64)
    endif()
endif ()

find_package(QT NAMES Qt5 COMPONENTS Widgets Bluetooth Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Bluetooth REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
        logger.cpp logger.h logger.ui
        widgets/listwidget.cpp widgets/listwidget.h widgets/listwidget.ui
        devices.cpp devices.h)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DataLogger
            ${PROJECT_SOURCES}
            )
else ()
    if (ANDROID)
        add_library(DataLogger SHARED
                ${PROJECT_SOURCES}
                )
    else ()
        add_executable(DataLogger
                ${PROJECT_SOURCES}
                )
    endif ()
endif ()

target_link_libraries(DataLogger PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Bluetooth
        Qt${QT_VERSION_MAJOR}::Network
        )
