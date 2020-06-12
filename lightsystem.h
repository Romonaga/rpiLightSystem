#ifndef LIGHTSYSTEM_H
#define LIGHTSYSTEM_H

#include <QObject>
#include <QThread>
#include <QVector>
#include <QMutex>

#include  <condition_variable>

#include "systemsettings.h"
#include "ws2811wrapper.h"
#include "mqttreceiver.h"
#include "dnrlogger.h"
#include "ilightshow.h"

//broker = "RomoServer.local";
//topic = "MotionDetect/#";
//qos = 1;



class LightSystem : QObject
{
    Q_OBJECT

public:
    explicit LightSystem(QObject *parent = nullptr);
    ~LightSystem();
    bool startSystem(); 
    void stopSystem();
    const char *getEnumName(int index);

    void runShow();

private:
    SystemSettings* _settings;
    void lightsOff();
    void stopShows();
    void startShows();
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

    void queueShow(const LedLightShows& show);

private:
    Ws2811Wrapper _ledWrapper;
    bool _started; //TODO: Replace with proper thead lock
    bool _running;
    MqttReceiver* _mqq;
    DNRLogger* _logger;
    QVector<ILightShow*> _runningShows;
    QMutex _runningShowsMutex;
    

public slots:
    void processMsgReceived(QString msg);
    void showComplete(ILightShow* show);


};


#endif // LIGHTSYSTEM_H
