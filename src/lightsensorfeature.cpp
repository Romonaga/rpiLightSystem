#include "lightsensorfeature.h"
#include <QSqlQuery>
#include <QDebug>
#include <time.h>
#include <wiringPi.h>



LightSensorFeature::LightSensorFeature(const QSqlQuery &qry)
{
    _logger = DNRLogger::instance();
    _settings = SystemSettings::getInstance();
    _noMotionTimer = 0;

    _featurePlayList = qry.value("featurePlayList").toUInt();
    _gpioPin = qry.value("featureGpio").toUInt();

    _logger->logInfo("LightSensorFeature Init.");

}

LightSensorFeature::~LightSensorFeature()
{
    stop();
    _logger->logInfo("LightSensorFeature Shutting Down.");
}

void LightSensorFeature::run()
{
    int pinRead;
    int pinLastState = 1;
    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);
    time_t nextTrip = 0;

     _logger->logInfo("LightSensorFeature run.");
    if(wiringPiSetupPhys() != 0)
    {
        _logger->logCritical("LightSensorFeature WiringPiSetup Failed");
        return;
    }

    _running = true;
    pinMode(_gpioPin, INPUT);

    while(_conditionVar.wait_for(lck,std::chrono::milliseconds(300)) == std::cv_status::timeout && _running == true) //todo remove hardcoded wait_for
    {
        if( (pinRead = digitalRead(_gpioPin)) != pinLastState)
        {
                if(nextTrip <= time(nullptr))
                {
                    pinLastState = pinRead;
                    emit lightSensorStateChange(this, pinLastState);
                    nextTrip = time(nullptr) + 120;
                }

        }

    }

    _running = false;
    emit lightSensorCompleted(this);

}

void LightSensorFeature::stop()
{
    if(_running)
    {
        _running = false;
        _conditionVar.notify_all();
        wait();
        _logger->logInfo("LightSensorFeature Stopped.");
    }
}


uint32_t LightSensorFeature::getFeaturePlayList() const
{
    return _featurePlayList;
}

