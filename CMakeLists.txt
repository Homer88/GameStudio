cmake_minimum_required(VERSION 3.16)
project(QtProjectManager VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/welcomescreen.cpp
    src/projectselectionscreen.cpp
    src/projectcreator.cpp
)

set(PROJECT_HEADERS
    src/mainwindow.h
    src/welcomescreen.h
    src/projectselectionscreen.h
    src/projectcreator.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QtProjectManager
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
    )
else()
    add_executable(QtProjectManager
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
    )
endif()

target_link_libraries(QtProjectManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

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
