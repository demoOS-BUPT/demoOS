#-------------------------------------------------
#
# Project created by QtCreator 2018-03-30T08:53:35
#
#-------------------------------------------------

QT       += core \
gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demoOS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += source/main.cpp\
        source/mainwindow.cpp \
    source/demo_process.cpp \
    source/list_op.cpp \
    source/FS/Directory.cpp \
    source/FS/DirOperate.cpp \
    source/FS/DiskOperate.cpp \
    source/FS/FCB.cpp \
    source/FS/other.cpp \
    source/RAM/FIFO.cpp \
    source/FS/instructionOP.cpp \
    source/FS/User.cpp \
    logindialog.cpp

HEADERS  += source/mainwindow.h \
   source/demo_process.h \
    source/list_op.h \
    source/RAM/firstfit.h \
    source/FS/Directory.h \
    source/FS/DirOperate.h \
    source/FS/DiskOperate.h \
    source/FS/FCB.h \
    source/FS/other.h \
    source/FS/instructionOp.h \
    source/FS/User.h \
    logindialog.h

FORMS    += source/mainwindow.ui \
    logindialog.ui

RESOURCES += \
    resource/res.qrc
