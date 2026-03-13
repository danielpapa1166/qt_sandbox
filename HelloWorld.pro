QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = hello_world
TEMPLATE = app

SOURCES += main.cpp mainwindow.cpp

HEADERS += mainwindow.h

# Use relative paths to bkk_api
BKK_API_PATH = ../bkk_api/cpp
BKK_API_LIB = $$BKK_API_PATH/libbkk_api_shared.so

# Library and include paths using relative paths
LIBS += -L$$BKK_API_PATH -lbkk_api_shared
INCLUDEPATH += $$BKK_API_PATH/include
INCLUDEPATH += $$BKK_API_PATH/_deps/nlohmann_json-src/include
INCLUDEPATH += $$BKK_API_PATH/_deps/nlohmann_json-src/single_include

# Link against curl (required by bkk_api)
unix {
    LIBS += -lcurl
    QMAKE_RPATHDIR += $$BKK_API_PATH
}

# Add CMake build step for bkk_api before building this app
bkk_api_build.target = $$BKK_API_LIB
bkk_api_build.commands = cd $$PWD/$$BKK_API_PATH && cmake -B . && make

PRE_TARGETDEPS += $$BKK_API_LIB
QMAKE_EXTRA_TARGETS += bkk_api_build  