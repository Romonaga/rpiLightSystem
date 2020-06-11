#ifndef LIGHTSYSTEM_H
#define LIGHTSYSTEM_H

#include <QObject>
#include "systemsettings.h"
#include "ws2811wrapper.h"
#include "mqttreceiver.h"
#include "dnrlogger.h"
#include <QThread>
#include <QVector>
#include  <condition_variable>

//broker = "RomoServer.local";
//topic = "MotionDetect/#";
//qos = 1;



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
    void colorThirds(ws2811_led_t startColor, ws2811_led_t middleColor, ws2811_led_t finishColor, u_int32_t delay);
    void colorForths(ws2811_led_t colorOne, ws2811_led_t colorTwo, ws2811_led_t colorThree,ws2811_led_t colorFour, u_int32_t delay);
    void triChaser(ws2811_led_t c1, ws2811_led_t c2, ws2811_led_t c3, u_int32_t delay);

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
   // QVector<int> _runShows;
    std::condition_variable _conditionVar;
    

public slots:
    void processMsgReceived(QString msg);

protected:
    void run();

};


#endif // LIGHTSYSTEM_H
