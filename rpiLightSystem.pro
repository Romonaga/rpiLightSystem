QT += core sql
QT -= gui

CONFIG += c++11

TARGET = rpiLightSystem
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
SOURCES += main.cpp \
    ilightshow.cpp \
    lightsystem.cpp \
    mqttreceiver.cpp \
    showblink.cpp \
    showchaser.cpp \
    showcolor3.cpp \
    showcolor3r.cpp \
    showcolor4.cpp \
    showcolorwipe.cpp \
    showcyclon.cpp \
    showfire.cpp \
    showhnh.cpp \
    showneorand.cpp \
    showrainbow.cpp \
    showrainbowcycle.cpp \
    showtheaterchaser.cpp \
    showtheaterchaserainbow.cpp \
    showtrichaser.cpp \
    systemsettings.cpp

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
    ilightshow.h \
    lightsystem.h \
    mqttreceiver.h \
    showblink.h \
    showchaser.h \
    showcolor3.h \
    showcolor3r.h \
    showcolor4.h \
    showcolorwipe.h \
    showcyclon.h \
    showfire.h \
    showhnh.h \
    showneorand.h \
    showrainbow.h \
    showrainbowcycle.h \
    showtheaterchaser.h \
    showtheaterchaserainbow.h \
    showtrichaser.h \
    systemsettings.h

LIBS += -lWs2811Wrapper
LIBS += -lws2811
LIBS += -lpaho-mqttpp3 -lpaho-mqtt3a
LIBS += -lMQTTMessageBus
LIBS += -lDNRLogger

unix {
    target.path = /usr/local/bin
    INSTALLS += target
}
