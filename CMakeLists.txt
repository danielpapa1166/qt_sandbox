cmake_minimum_required(VERSION 3.16)
project(qt_sandbox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt meta-object code generation for classes using Q_OBJECT.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Build the bkk_api library from the submodule/repo in-tree.
add_subdirectory(bkk_api/cpp)

add_executable(hello_world
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(hello_world
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        bkk_api_shared
)
