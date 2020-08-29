QT += core sql
CONFIG += console
CONFIG -= app_bundle
QT -= gui

CONFIG += c++11

TARGET = rpiLightSystem

TEMPLATE = app
SOURCES += src/ilightshow.cpp \
    src/channelsettings.cpp \
    src/gifdec.c \
    src/jpgd.cpp \
    src/lightluxfeature.cpp \
    src/lightsensorfeature.cpp \
    src/lightsystem.cpp \
    src/lodepng.cpp \
    src/main.cpp \
    src/matrixantart.cpp \
    src/matrixart.cpp \
    src/matrixcolorevolution.cpp \
    src/matrixcreatejpg.cpp \
    src/matrixdisplayfile.cpp \
    src/matrixgameoflife.cpp \
    src/matrixkaleidoscope.cpp \
    src/matrixmazecreator.cpp \
    src/matrixscrolltext.cpp \
    src/matrixscrolltime.cpp \
    src/matrixsquare.cpp \
    src/matrixtcircle.cpp \
    src/matrixtriangle.cpp \
    src/matrixtrip.cpp \
    src/matrixvolbars.cpp \
    src/motionlightsfeature.cpp \
    src/mqttreceiver.cpp \
    src/playlistmanager.cpp \
    src/showblink.cpp \
    src/showbouncingballs.cpp \
    src/showchaser.cpp \
    src/showcolor.cpp \
    src/showcolor3.cpp \
    src/showcolor3r.cpp \
    src/showcolor4.cpp \
    src/showcolorevery.cpp \
    src/showcolorwipe.cpp \
    src/showcyclon.cpp \
    src/showdelay.cpp \
    src/showfade.cpp \
    src/showflame.cpp \
    src/showhnh.cpp \
    src/showneorand.cpp \
    src/showpulse.cpp \
    src/showrainbow.cpp \
    src/showrainbowcycle.cpp \
    src/showscanner.cpp \
    src/showtheaterchaser.cpp \
    src/showtheaterchaserainbow.cpp \
    src/showtrichaser.cpp \
    src/showtwinkle.cpp \
    src/systemsettings.cpp  \
    src/timefeature.cpp \
    src/toojpeg.cpp

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
    src/channelsettings.h \
    src/gifdec.h \
    src/ilightshow.h \
    src/jpgd.h \
    src/lightluxfeature.h \
    src/lightsensorfeature.h \
    src/lightsystem.h \
    src/lodepng.h \
    src/matrixantart.h \
    src/matrixart.h \
    src/matrixcircle.h \
    src/matrixcolorevolution.h \
    src/matrixcreatejpg.h \
    src/matrixdisplayfile.h \
    src/matrixgameoflife.h \
    src/matrixkaleidoscope.h \
    src/matrixmazecreator.h \
    src/matrixscrolltext.h \
    src/matrixscrolltime.h \
    src/matrixsquare.h \
    src/matrixtriangle.h \
    src/matrixtrip.h \
    src/matrixvolbars.h \
    src/motionlightsfeature.h \
    src/mqttreceiver.h \
    src/pixalfonts.h \
    src/playlistmanager.h \
    src/showblink.h \
    src/showbouncingballs.h \
    src/showchaser.h \
    src/showcolor.h \
    src/showcolor3.h \
    src/showcolor3r.h \
    src/showcolor4.h \
    src/showcolorevery.h \
    src/showcolorwipe.h \
    src/showcyclon.h \
    src/showdelay.h \
    src/showfade.h \
    src/showflame.h \
    src/showhnh.h \
    src/showneorand.h \
    src/showpulse.h \
    src/showrainbow.h \
    src/showrainbowcycle.h \
    src/showscanner.h \
    src/showtheaterchaser.h \
    src/showtheaterchaserainbow.h \
    src/showtrichaser.h \
    src/showtwinkle.h \
    src/systemsettings.h \
    src/timefeature.h \
    src/toojpeg.h \


LIBS += -lWs2811Wrapper
LIBS += -lws2811
LIBS += -lpaho-mqttpp3 -lpaho-mqtt3a
LIBS += -lMQTTMessageBus
LIBS += -lDNRLogger
LIBS += -lwiringPi
LIBS += -li2c
LIBS += -lrgbmatrix

unix {
    target.path = /lumawin
    INSTALLS += target
}
