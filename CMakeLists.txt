cmake_minimum_required(VERSION 3.16)
project(QtProjectManager VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Xml SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Xml SerialPort)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/settings)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/welcomescreen.cpp
    src/projectselectionscreen.cpp
    src/projectcreator.cpp
    src/language_manager.cpp
    src/settings/settingsdialog.cpp
    src/codeeditor.cpp
)

set(PROJECT_HEADERS
    src/mainwindow.h
    src/welcomescreen.h
    src/projectselectionscreen.h
    src/projectcreator.h
    src/language_manager.h
    src/settings/settingsdialog.h
    src/codeeditor.h
)

set(PROJECT_UI
    src/settings/settingsdialog.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QtProjectManager
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_UI}
    )
else()
    add_executable(QtProjectManager
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_UI}
    )
endif()

target_link_libraries(QtProjectManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Xml Qt${QT_VERSION_MAJOR}::SerialPort)

set_target_properties(QtProjectManager PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QtProjectManager)
endif()
