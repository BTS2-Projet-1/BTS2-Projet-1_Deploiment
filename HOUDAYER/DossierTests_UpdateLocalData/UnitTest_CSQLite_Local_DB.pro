#-------------------------------------------------
#
# Project created by QtCreator 2015-05-18T20:24:44
#
#-------------------------------------------------

QT       += sql testlib
QT       -= gui

TARGET = unittest_csqlite_local_db
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += unittest_csqlite_local_db.cpp \
    clms_db.cpp \
    csqlite_local_db.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    clms_db.h \
    csqlite_local_db.h \
    DataStructures.h
