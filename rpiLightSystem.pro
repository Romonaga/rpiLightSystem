QT += core sql
QT -= gui

CONFIG += c++11

TARGET = rpiLightSystem
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
SOURCES += src/ilightshow.cpp \
    src/lightsystem.cpp \
    src/main.cpp \
    src/mqttreceiver.cpp \
    src/playlistmanager.cpp \
    src/showPulse.cpp \
    src/showblink.cpp \
    src/showchaser.cpp \
    src/showcolor.cpp \
    src/showcolor3.cpp \
    src/showcolor3r.cpp \
    src/showcolor4.cpp \
    src/showcolorevery.cpp \
    src/showcolorwipe.cpp \
    src/showcyclon.cpp \
    src/showflame.cpp \
    src/showhnh.cpp \
    src/showneorand.cpp \
    src/showrainbow.cpp \
    src/showrainbowcycle.cpp \
    src/showtheaterchaser.cpp \
    src/showtheaterchaserainbow.cpp \
    src/showtrichaser.cpp \
    src/showtwinkle.cpp \
    src/systemsettings.cpp 

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    src/ilightshow.h \
    src/lightsystem.h \
    src/mqttreceiver.h \
    src/playlistmanager.h \
    src/showPulse.h \
    src/showblink.h \
    src/showchaser.h \
    src/showcolor.h \
    src/showcolor3.h \
    src/showcolor3r.h \
    src/showcolor4.h \
    src/showcolorevery.h \
    src/showcolorwipe.h \
    src/showcyclon.h \
    src/showflame.h \
    src/showhnh.h \
    src/showneorand.h \
    src/showrainbow.h \
    src/showrainbowcycle.h \
    src/showtheaterchaser.h \
    src/showtheaterchaserainbow.h \
    src/showtrichaser.h \
    src/showtwinkle.h \
    src/systemsettings.h 

LIBS += -lWs2811Wrapper
LIBS += -lws2811
LIBS += -lpaho-mqttpp3 -lpaho-mqtt3a
LIBS += -lMQTTMessageBus
LIBS += -lDNRLogger

unix {
    target.path = /lumawin
    INSTALLS += target
}
