#include "lightsensorfeature.h"
#include <QDebug>
#include <time.h>
#include <wiringPi.h>



LightSensorFeature::LightSensorFeature()
{
    _logger = DNRLogger::instance();
    _settings = SystemSettings::getInstance();
    _pinLastState = -1;
    _noMotionTimer = 0;
    stop();
    _logger->logInfo("LightSensorFeature Shutting Down.");

}

void LightSensorFeature::run()
{
    int pinRead;
    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);

     _logger->logInfo("LightSensorFeature run.");
    if(wiringPiSetupPhys() != 0)
    {
        _logger->logCritical("LightSensorFeature WiringPiSetup Failed");
        return;
    }

    _running = true;
    pinMode(_settings->getLightFeatureGpio(), INPUT);

    while(_conditionVar.wait_for(lck,std::chrono::milliseconds(300)) == std::cv_status::timeout && _running == true) //todo remove hardcoded wait_for
    {
        if( (pinRead = digitalRead(_settings->getLightFeatureGpio())) != _pinLastState)
        {
                _pinLastState = pinRead;
                emit lightSensorStateChange(this, _pinLastState);

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
        _logger->logInfo("LightSensorFeature Stopped.");
    }
}
