#-------------------------------------------------
#
# Project created by QtCreator 2016-03-28T12:02:31
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = modbus_parser
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serialport.cpp \
    serialsettingdialog.cpp \
    serialsenddialog.cpp

HEADERS  += mainwindow.h \
    serialport.h \
    serialsettingdialog.h \
    serialsenddialog.h

FORMS    += mainwindow.ui \
    serialsettingdialog.ui \
    serialsenddialog.ui
