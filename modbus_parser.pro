#-------------------------------------------------
#
# Project created by QtCreator 2016-03-28T12:02:31
#
#-------------------------------------------------

QT       += core gui serialbus serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = modbus_parser
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serialsettingdialog.cpp \
    serialsenddialog.cpp \
    modbus.cpp

HEADERS  += mainwindow.h \
    serialsettingdialog.h \
    serialsenddialog.h \
    modbus.h

FORMS    += mainwindow.ui \
    serialsettingdialog.ui \
    serialsenddialog.ui
