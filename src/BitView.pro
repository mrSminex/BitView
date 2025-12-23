#-------------------------------------------------
#
# Project created by QtCreator 2025-11-15T11:02:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mattress-2025-2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    DemultStreamDialog.cpp \
    MVC/TextDelegate.cpp \
        main.cpp \
        mainwindow.cpp \
    AboutDialog.cpp \
    MVC/Model.cpp \
    MVC/BaseDelegate.cpp \
    MVC/BitDelegate.cpp \
    MVC/Viewer.cpp \
    MVC/HexDelegate.cpp

HEADERS += \
    DemultStreamDialog.h \
    MVC/TextDelegate.h \
        mainwindow.h \
    AboutDialog.h \
    MVC/Model.h \
    MVC/BaseDelegate.h \
    MVC/BitDelegate.h \
    MVC/Viewer.h \
    MVC/HexDelegate.h

FORMS += \
    DemultStreamDialog.ui \
        mainwindow.ui \
    AboutDialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    q.qrc

RC_ICONS = ../img/mattress.ico



INCLUDEPATH += MVC








