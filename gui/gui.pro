QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gesture-recognition
TEMPLATE = app


SOURCES += \
    main.cpp\
    mainwindow.cpp \
    drawablearea.cpp \
    uiinteractor.cpp

HEADERS  += \
    mainwindow.h \
    drawablearea.h \
    uiinteractor.h

INCLUDEPATH += ../librecog
LIBS += -L../librecog -lrecog

FORMS    += ui/mainwindow.ui
