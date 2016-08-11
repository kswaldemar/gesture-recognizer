QT       += core gui

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

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
LIBS += ../librecog/librecog.a
PRE_TARGETDEPS += ../librecog/librecog.a

FORMS    += ui/mainwindow.ui
