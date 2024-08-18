#-------------------------------------------------
#
# Project created by QtCreator 2016-11-27T12:21:47
#
#-------------------------------------------------

QT       += core gui multimedia sql svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FountainPlayer
TEMPLATE = app

#linux {
#    contains(QMAKE_HOST.arch, arm.*):{
#        LIBS += -lbcm2835
#    }
#}


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
    src/hardware/RPI.c \
    src/ui/dmxdebugform.cpp \
    src/ui/iodebugform.cpp \
    src/core/digitaliointerface.cpp \
    src/core/digitaliointerfaceclient.cpp \
    src/core/digialinputresponse.cpp \
    src/core/addressbook.cpp \
    src/core/dmxinterface.cpp \
    src/core/proceduremanager.cpp \
    src/ui/stateview.cpp \
    src/ui/procedureviewform.cpp \
    src/ui/lockscreenform.cpp \
    src/ui/changepassworddialog.cpp \
    src/ui/imageframe.cpp \
    src/ui/dividerwidget.cpp \
    src/ui/numpadwidget.cpp

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
    src/hardware/RPI.h \
    src/ui/dmxdebugform.h \
    src/ui/iodebugform.h \
    src/core/digitaliointerface.h \
    src/flags/projectflags.h \
    src/core/digitaliointerfaceclient.h \
    src/core/digialinputresponse.h \
    src/core/addressbook.h \
    src/core/dmxinterface.h \
    src/core/proceduremanager.h \
    src/ui/stateview.h \
    src/ui/procedureviewform.h \
    src/ui/lockscreenform.h \
    src/ui/changepassworddialog.h \
    src/ui/imageframe.h \
    src/ui/dividerwidget.h \
    src/ui/numpadwidget.h

FORMS    += src/ui/mainwindow.ui \
    src/ui/addtoplaylistdialog.ui \
    src/ui/dmxdebugform.ui \
    src/ui/iodebugform.ui \
    src/ui/procedureviewform.ui \
    src/ui/lockscreenform.ui \
    src/ui/changepassworddialog.ui \
    src/ui/numpadwidget.ui

RESOURCES += \
    res/qdarkstyle/style.qrc \
    resource.qrc
