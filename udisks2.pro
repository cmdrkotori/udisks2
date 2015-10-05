#-------------------------------------------------
#
# Project created by QtCreator 2015-10-04T11:31:02
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += C++11
TARGET = udisk2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    udisks2.cpp

HEADERS  += mainwindow.h \
    udisks2.h

FORMS    += mainwindow.ui
