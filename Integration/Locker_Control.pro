#-------------------------------------------------
#
# Project created by QtCreator 2015-05-03T18:30:16
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += xml


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Locker_Control
TEMPLATE = app


SOURCES += \
    boxchoice.cpp \
    cdoors.cpp \
    clms_db.cpp \
    consolewindows.cpp \
    country.cpp \
    csqlite_local_db.cpp \
    customer.cpp \
    custompushbutton.cpp \
    customtreewidget.cpp \
    customxml.cpp \
    deliveryconfirm.cpp \
    doorsmanager.cpp \
    drawbox.cpp \
    keyboard.cpp \
    login.cpp \
    main.cpp \
    maintenancemenu.cpp \
    menudelivery.cpp \
    openemptybox.cpp \
    scanpackage.cpp \
    setting.cpp \
    settingsystem.cpp \
    showpackagebox.cpp \
    showwidgets.cpp \
    sqlite_local_databasemanager.cpp \
    stowpackages.cpp \
    thread_databasemanager.cpp \
    thread_localdatabase_autosave.cpp \
    thread_statusdoors.cpp \
    threaddeleteextractcode.cpp \
    threadsavedelivery.cpp \
    assistance.cpp \
    editsettingsystem.cpp \
    siteconfigmanager.cpp \
    thread_linphonec.cpp

HEADERS  += \
    boxchoice.h \
    cdoors.h \
    clms_db.h \
    consolewindows.h \
    country.h \
    csqlite_local_db.h \
    customer.h \
    custompushbutton.h \
    customtreewidget.h \
    customxml.h \
    DataStructures.h \
    deliveryconfirm.h \
    doorsmanager.h \
    drawbox.h \
    keyboard.h \
    login.h \
    maintenancemenu.h \
    menudelivery.h \
    scanpackage.h \
    setting.h \
    settingsystem.h \
    showpackagebox.h \
    showwidgets.h \
    openemptybox.h \
    sqlite_local_databasemanager.h \
    stowpackages.h \
    thread_databasemanager.h \
    thread_localdatabase_autosave.h \
    thread_statusdoors.h \
    threaddeleteextractcode.h \
    threadsavedelivery.h \
    assistance.h \
    editsettingsystem.h \
    siteconfigmanager.h \
    thread_linphonec.h

FORMS    += \
    boxchoice.ui \
    consolewindows.ui \
    customer.ui \
    deliveryconfirm.ui \
    doorsmanager.ui \
    keyboard.ui \
    login.ui \
    maintenancemenu.ui \
    menudelivery.ui \
    scanpackage.ui \
    setting.ui \
    settingsystem.ui \
    showpackagebox.ui \
    openemptybox.ui \
    sqlite_local_databasemanager.ui \
    stowpackages.ui \
    assistance.ui \
    editsettingsystem.ui \
    siteconfigmanager.ui

RESOURCES += \
    Src.qrc

TRANSLATIONS = english.ts french.ts
