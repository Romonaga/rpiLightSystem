#ifndef ILIGHTSHOW_H
#define ILIGHTSHOW_H

#include <QThread>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "ws2811wrapper.h"
#include "systemsettings.h"
#include "dnrlogger.h"


#include  <condition_variable>


#define LIGHT_SHOWS(X)                                                  \
            X(0, Nope, "Why Are We Here?","nope"),                           \
            X(1, Blink, "Blink","blink"),                           \
            X(2, Chaser, "Chaser","c"),                              \
            X(3, TC, "Theater Chase","tc"),                                    \
            X(4, TCR, "Theater Chase Rainbow","tcrb"),                        \
            X(5, Color3R, "Color 3 Reverse","c3r"),   \
            X(6, Cyclon, "Cylon","cy"),                          \
            X(7, ColorWipe, "Color Wipe","cw"),                    \
            X(8, HAndH, "Half n Half","hnh"),                    \
            X(9, Rainbow, "Rainbow","rb"),                   \
            X(10, RainbowCycle, "Rainbow Cycle","rbc"),                   \
            X(11, NeoRand, "NeoRand (Twinkle)","nrt"),                   \
            X(12, Flame, "Flame","f"),                   \
            X(13, ColorThirds, "ColorThirds","c3rd"),                   \
            X(14, ColorForths, "ColorForths","c4th"),                   \
            X(15, TriChaser, "Tri-Color Chase","trcc"),                   \
            X(16, DisplayColor, "Display A Color","dc"),                   \
            X(17, ColorEvery, "Color Every X Led.","cx"),                   \
            X(18, TwinkleOverlay, "Twinkle Overlay.","to"),                   \
            X(19, PulseOverlay, "Pulse Overlay.","po"),                   \
            X(20, Scanner, "Scanner.","scan"),                   \
            X(21, BBalls, "Bouncing Balls.","bb"),                   \
            X(22, Fade, "Fade.","fade")                   \


#define LIGHT_SHOWS_ENUM(type, name, txt, twitch) name = type
#define LIGHT_SHOWS_SHOW(type, name, txt, twitch) txt
#define LIGHT_SHOWS_TWITCH(type, name, txt, twitch) twitch

//{"UserID":"2","brightness":"127","show":"8","colors":{"color1":{"r":234,"g":10,"b":34},"color2":{"r":14,"g":64,"b":239}},"gammaCorrection":1}
//[{"show": "8", "UserID": "2",               "colors": {"color1": {"b": 255, "g": 0, "r": 18}, "color2": {"b": 250, "g": 107, "r": 144}}, "powerOn": 0, "brightness": "70", "clearStart": 0, "clearFinish": 0, "gammaCorrection": 1}]

typedef enum {
    LIGHT_SHOWS(LIGHT_SHOWS_ENUM),

    LIGHT_SHOWS_ENUM_COUNT
} LedLightShows;


class ILightShow : public QThread
{
    Q_OBJECT

public:
    explicit ILightShow(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString& showParms);
    void stopShow();


    LedLightShows getLightShow() const;
    QString getShowName();

    QString getShowParms() const;

    bool isRunning() const;
    int genRand(unsigned int min, unsigned int max);

    int getUserId() const;
    void gammaCorrection();



private:
    virtual void startShow() = 0;
    ws2811_led_t gamaColor(ws2811_led_t inColor);


protected:
    SystemSettings* _settings;
    Ws2811Wrapper* _ledWrapper;
    LedLightShows _lightShow;
    QString _showParms;
    int _userId;


    bool _running;
    DNRLogger* _logger;

    bool _clearOnStart;
    bool _clearOnFinish;
    bool _useGammaCorrection;
    uint8_t  _gammaCorrection[256];

    ws2811_led_t _color1;
    ws2811_led_t _color2;
    ws2811_led_t _color3;
    ws2811_led_t _color4;
    uint8_t _brightness;
    time_t _endTime;

    double _numMins;
    uint8_t _width;

    uint16_t _wait;
    uint16_t _colorEvery;

    int _r;
    int _g;
    int _b;



protected:
    void run();


signals:
    void showComplete(ILightShow*);

};

Q_DECLARE_INTERFACE(ILightShow,"ILightShow")
#endif // ILIGHTSHOW_H



