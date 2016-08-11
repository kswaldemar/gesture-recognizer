QT += core gui

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

TARGET = recog
TEMPLATE = lib
CONFIG += staticlib

SOURCES += recognition.cpp shape.cpp houghtransform.cpp common.cpp
HEADERS += recognition.h shape.h houghtransform.h common.h
