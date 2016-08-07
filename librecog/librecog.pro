QT += core gui

TARGET = recog
TEMPLATE = lib
CONFIG += staticlib

SOURCES += recognition.cpp shape.cpp houghtransform.cpp common.cpp
HEADERS += recognition.h shape.h houghtransform.h common.h
