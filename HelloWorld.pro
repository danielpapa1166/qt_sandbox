QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = hello_world
TEMPLATE = app

SOURCES += main.cpp mainwindow.cpp

HEADERS += mainwindow.h

# add bkk_api library to the project for testing
# hardcoded for now: 
LIBS += -L/data/projects/bkk_api/cpp -lbkk_api_shared
INCLUDEPATH += /data/projects/bkk_api/cpp/include
INCLUDEPATH += /data/projects/bkk_api/cpp/_deps/nlohmann_json-src/include
INCLUDEPATH += /data/projects/bkk_api/cpp/_deps/nlohmann_json-src/single_include

# Link against curl (required by bkk_api)
unix {
    LIBS += -lcurl
    # Set runtime path to find libbkk_api_shared.so
    QMAKE_RPATHDIR += /data/projects/bkk_api/cpp
}  