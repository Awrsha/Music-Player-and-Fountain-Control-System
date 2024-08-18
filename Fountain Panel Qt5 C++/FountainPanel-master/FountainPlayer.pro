#-------------------------------------------------
#
# Project created by QtCreator 2016-11-27T12:21:47
#
#-------------------------------------------------

QT       += core gui multimedia sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FountainPlayer
TEMPLATE = app


SOURCES += src/core/main.cpp\
        src/ui/mainwindow.cpp \
    src/ui/bottombar.cpp \
    src/ui/musicpage.cpp \
    src/core/data.cpp \
    src/core/myplayer.cpp \
    src/ui/iconbutton.cpp \
    src/ui/listlist.cpp \
    src/ui/mymenu.cpp \
    src/ui/sliderwidget.cpp \
    src/ui/voiceslider.cpp \
    src/ui/qiconbutton.cpp \
    src/core/bringup.cpp \
    src/core/configurationmanager.cpp \
    src/util/utils.cpp \
    src/core/fountainplayer.cpp \
    src/ui/addtoplaylistdialog.cpp \
    src/core/mediainfo.cpp \
    src/ui/musicfountainlist.cpp \
    src/hardware/RPI_DMX.c \
    src/hardware/RPI.c

HEADERS  += src/ui/mainwindow.h \
    src/ui/bottombar.h \
    src/ui/musicpage.h \
    src/core/data.h \
    src/core/myplayer.h \
    src/core/playmode.h \
    src/ui/iconbutton.h \
    src/ui/listlist.h \
    src/ui/mymenu.h \
    src/ui/sliderwidget.h \
    src/ui/voiceslider.h \
    src/ui/qiconbutton.h \
    src/core/bringup.h \
    src/core/configurationmanager.h \
    src/util/projecttheme.h \
    src/util/utils.h \
    src/core/fountainplayer.h \
    src/ui/addtoplaylistdialog.h \
    src/core/mediainfo.h \
    src/ui/musicfountainlist.h \
    src/hardware/RPI.h

FORMS    += src/ui/mainwindow.ui \
    src/ui/addtoplaylistdialog.ui

RESOURCES += \
    res/qdarkstyle/style.qrc \
    resource.qrc
