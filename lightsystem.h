#ifndef LIGHTSYSTEM_H
#define LIGHTSYSTEM_H

#include <QObject>
#include "systemsettings.h"
#include "ws2811wrapper.h"
#include "mqttreceiver.h"
#include "dnrlogger.h"
#include <QThread>
#include <QVector>

//broker = "RomoServer.local";
//topic = "MotionDetect/#";
//qos = 1;

#define LIGHT_SHOWS(X)                                                  \
            X(0, Nope, "Why Are We Here?"),                           \
            X(1, Blink, "Blink"),                           \
            X(2, Chaser, "Chaser"),                              \
            X(3, TC, "Theater Chase"),                                    \
            X(4, TCR, "Theater Chase Rainbow"),                        \
            X(5, Color3R, "Color 3 Reverse"),   \
            X(6, Cylon, "Cylon"),                          \
            X(7, ColorWipe, "Color Wipe"),                    \
            X(8, HAndH, "Half n Half"),                    \
            X(9, Rainbow, "Rainbow"),                   \
            X(10, RainbowCycle, "Rainbow Cycle"),                   \
            X(11, NeoRand, "NeoRand (Twinkle)"),                   \
            X(12, Flame, "Flame")                   \


#define LIGHT_SHOWS_ENUM(type, name, str) name = type
#define LIGHT_SHOWS_STRING(type, name, str) str

typedef enum {
    LIGHT_SHOWS(LIGHT_SHOWS_ENUM),

    LIGHT_SHOWS_ENUM_COUNT
} LedLightShows;


class LightSystem : public QThread
{
    Q_OBJECT

public:
    explicit LightSystem();
    ~LightSystem();
    bool startSystem();
    void stopSystem();
    const char *getEnumName(int index);

private:
    SystemSettings* _settings;
    void lightsOff();
    void stopLights();
    unsigned int random9();
    uint32_t Bow(int n);

private: //shows
    void chaser(u_int32_t wait);
    void theaterChase(ws2811_led_t baseColor, u_int32_t wait);

    void theaterChaseRainbow(u_int32_t wait);
    void flame();
    void neoRand();
    void colorThirdsReverse(uint32_t startColor, uint32_t middleColor, uint32_t finishColor, int wait);
    void blink(int numBlinks, int wait);
    void cyclon(ws2811_led_t c, int width, int speed);
    void colorWipe(ws2811_led_t color, u_int32_t waitms);
    void halfnHalf(ws2811_led_t halfN, ws2811_led_t nHalf, u_int32_t delayMs);
    void rainbow(u_int32_t wait);
    void rainbowCycle(u_int32_t wait);


private:
    Ws2811Wrapper _ledWrapper;
    bool _started; //TODO: Replace with proper thead lock
    bool _running;
    int16_t _red;
    int16_t _green;
    int16_t _blue;
    unsigned int _rno;
    unsigned int _cno;
    int _tw;
    int _wait;
    MqttReceiver* _mqq;
    DNRLogger* _logger;
    QVector<int> _runShows;


public slots:
    void processMsgReceived(QString msg);

protected:
    void run();

};


#endif // LIGHTSYSTEM_H
