#-------------------------------------------------
#
# Project created by QtCreator 2016-08-03T18:32:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gesture-recognition-widgets
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    drawablearea.cpp \
    guihandler.cpp

HEADERS  += mainwindow.h \
    drawablearea.h \
    guihandler.h

FORMS    += mainwindow.ui
