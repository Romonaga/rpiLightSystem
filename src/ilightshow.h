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
            X(0, Nope, "Why Are We Here?"),                           \
            X(1, Blink, "Blink"),                           \
            X(2, Chaser, "Chaser"),                              \
            X(3, TC, "Theater Chase"),                                    \
            X(4, TCR, "Theater Chase Rainbow"),                        \
            X(5, Color3R, "Color 3 Reverse"),   \
            X(6, Cyclon, "Cylon"),                          \
            X(7, ColorWipe, "Color Wipe"),                    \
            X(8, HAndH, "Half n Half"),                    \
            X(9, Rainbow, "Rainbow"),                   \
            X(10, RainbowCycle, "Rainbow Cycle"),                   \
            X(11, NeoRand, "NeoRand (Twinkle)"),                   \
            X(12, Flame, "Flame"),                   \
            X(13, ColorThirds, "ColorThirds"),                   \
            X(14, ColorForths, "ColorForths"),                   \
            X(15, TriChaser, "Tri-Color Chase"),                   \
            X(16, DisplayColor, "Display A Color"),                   \
            X(17, ColorEvery, "Color Every X Led."),                   \
            X(18, TwinkleOverlay, "Twinkle Overlay."),                   \
            X(19, PulseOverlay, "Pulse Overlay."),                   \
            X(20, Scanner, "Scanner."),                   \
            X(21, BBalls, "Bouncing Balls."),                   \
            X(22, Fade, "Fade."),                   \
            X(23, MatrixArtShow, "Display Art"),                   \
            X(24, MatrixText, "Scroll Text"),                   \
            X(25, DelayShow, "Delay Show"),                   \
            X(26, MatrixTripShow, "Trippy Matrix"),                   \
            X(27, MatrixC, "Circle")                   \


#define LIGHT_SHOWS_ENUM(type, name, txt) name = type
#define LIGHT_SHOWS_SHOW(type, name, txt) txt


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
    int getUserId() const;


private:
    virtual void startShow() = 0;
    ws2811_led_t gamaColor(ws2811_led_t inColor);
    void drawCircle(int xc, int yc, int x, int y);


protected:
    int genRand(unsigned int min, unsigned int max);
    void gammaCorrection();
    void circleBres(int xc, int yc, int r);




protected:
    SystemSettings* _settings;
    Ws2811Wrapper* _ledWrapper;
    LedLightShows _lightShow;
    QString _showParms;
    QString _matrixText;
    int _userId;
     QJsonObject _showParmsJson;


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



