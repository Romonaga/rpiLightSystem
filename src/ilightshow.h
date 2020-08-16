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

#include "font7x6ext.h"

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
            X(27, MatrixC, "Circle"),                   \
            X(28, MatrixS, "Square"),                   \
            X(29, MatrixT, "Triangle"),                   \
            X(30, MatrixJpg, "Create Art JPG"),                   \
            X(31, MatrixDF, "Display File JPG/PNG")                   \


#define LIGHT_SHOWS_ENUM(type, name, txt) name = type
#define LIGHT_SHOWS_SHOW(type, name, txt) txt


typedef enum {
    LIGHT_SHOWS(LIGHT_SHOWS_ENUM),

    LIGHT_SHOWS_ENUM_COUNT
} LedLightShows;

typedef enum {
    PointUp = 1,
    PointRight = 2,
    PointDown =   3,
    PointLeft = 4

} TrigDirection;

class ILightShow : public QThread
{
    Q_OBJECT

public:
    explicit ILightShow(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString& showParms);
    ~ILightShow();
    void stopShow();


    LedLightShows getLightShow() const;
    QString getShowName();
    QString getShowParms() const;
    bool isRunning() const;
    int getUserId() const;


private:
    virtual void startShow() = 0;
    ws2811_led_t gamaColor(ws2811_led_t inColor);
    void circleBres(int xc, int yc, int r);
    void drawCircle(int xc, int yc, int x, int y);
    void circlePoints(int cx, int cy, int x, int y);
    void circleMidpoint(int xCenter, int yCenter, int radius);




protected:
    int genRand(unsigned int min, unsigned int max);
    void gammaCorrection();

    void drawCircle(int xCenter, int yCenter, int radius);
    void drawBox(int startRow, int startcol, int length, int height);
    void drawline(int x1, int y1, int x2, int y2);
    void drawTriangle(int startRow, int startCol, int size, int direction);

    void shiftColumns(int maxRows, int rowStart, ws2811_led_t color, ws2811_led_t * snapShotBuffer);
    void scrollText(QString msg, int maxRows, int maxCols, int rowStart, int drawCol, ws2811_led_t color,  ws2811_led_t * snapShotBuffer, int delay);

    //You ARE responsable for this memory allocation!
    ws2811_led_t* snapShot(int rowStart, int maxRows, uint32_t *snapShotBufferSize);

    void replaySnapShot(int rowStart, int maxRows, ws2811_led_t* snapShotBuffer);

    //You ARE responsable for this memory allocation!
    unsigned char* resampleRGB(int newWidth, int newHeight, int width, int height, unsigned char* imageData);
    ws2811_led_t* resampleColor(int newWidth,  int newHeight, int width, int height, ws2811_led_t* imageData);

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
    short _channelId;


    double _numMins;
    uint8_t _width;

    uint16_t _wait;
    uint16_t _colorEvery;

    int _r;
    int _g;
    int _b;

    int _rowStart;
    int _drawCol;


protected:
    void run();


signals:
    void showComplete(ILightShow*);

};

Q_DECLARE_INTERFACE(ILightShow,"ILightShow")
#endif // ILIGHTSHOW_H



