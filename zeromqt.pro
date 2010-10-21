#-------------------------------------------------
#
# Project created by QtCreator 2010-10-15T17:00:35
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = pubsubsample
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cc \
    ZmqContext.cc \
    ZmqSocket.cc \
    App.cc

HEADERS += \
    ZmqContext.h \
    ZmqSocket.h \
    ZmqMessage.h \
    ZmqException.h \
    App.h

LIBS += -lzmq
