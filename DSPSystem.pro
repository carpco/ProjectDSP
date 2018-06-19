#-------------------------------------------------
#
# Project created by QtCreator 2017-11-28T23:16:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pruebas
TEMPLATE = app

LIBS += -lfftw3 -ljack -lsndfile -lGL

INCLUDEPATH += /usr/include


SOURCES += main.cpp\
        mainwindow.cpp \
    controlvolume.cpp \
    dspsystem.cpp \
    jack.cpp

HEADERS  += mainwindow.h \
    controlvolume.h \
    dspsystem.h \
    jack.h \
    processor.h \
    spectralvalues.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
