#ifndef LightLuxFeature_H
#define LightLuxFeature_H

#include <QThread>
#include <QString>
#include <QSqlQuery>

#include <time.h>
#include  <condition_variable>

#include "systemsettings.h"
#include "dnrlogger.h"



#define BH1750FVI_I2C_ADDRESS 0x23
enum BH1750FVI
{
    PowerDown    = 0x00,
    PowerOn      = 0x01,
    Reset        = 0x07,
    ContinueHigh = 0x10,
    ContinueLow  = 0x13,
    OneTimeHigh  = 0x20,
    OneTimeLow   = 0x23

};

//#define PowerDown     0x00
//#define PowerOn     0x01
//#define Reset        0x07
//#define ContinueHigh    0x10
//#define ContinueLow    0x13
//#define OneTimeHigh    0x20
//#define OneTimeLow    0x23

class LightLuxFeature : public QThread
{
    Q_OBJECT

public:
    explicit LightLuxFeature(const QSqlQuery& qry, BH1750FVI cmd);
    ~LightLuxFeature();
    void stop();
    uint32_t getFeaturePlayList() const;

    uint32_t getLuxThreshHold() const;

private:
    uint32_t _lastLux;

protected:
    SystemSettings* _settings;
    bool _running;
    DNRLogger* _logger;
    std::condition_variable _conditionVar;
    uint32_t _featurePlayList;
    short _gpioPin;
    uint32_t _luxThreshHold;
    BH1750FVI _cmd;



protected:
    void run();


signals:
    void lightLuxCompleted(LightLuxFeature*);
    void lightLuxStateChange(LightLuxFeature*, quint32);

};


#endif // LightLuxFeature_H

