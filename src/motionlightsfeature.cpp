#include "motionlightsfeature.h"
#include <QDebug>
#include <time.h>
#include <wiringPi.h>



MotionLightsFeature::MotionLightsFeature()
{
    _logger = DNRLogger::instance();
    _settings = SystemSettings::getInstance();
    _pinLastState = 0;
    _noMotionTimer = 0;
    stop();
    _logger->logInfo("MotionLightsFeature Shutting Down.");

}

MotionLightsFeature::~MotionLightsFeature()
{
    stop();
    _logger->logInfo("MotionLightsFeature Shutting Down.");
}

void MotionLightsFeature::run()
{
    int pinRead;
    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);

     _logger->logInfo("MotionLightsFeature run.");
    if(wiringPiSetupPhys() != 0)
    {
        _logger->logCritical("MotionLightsFeature WiringPiSetup Failed");
        return;
    }

    _running = true;
    pinMode(_settings->getMotionFeatureGpio(), INPUT);

    while(_conditionVar.wait_for(lck,std::chrono::milliseconds(300)) == std::cv_status::timeout && _running == true) //todo remove hardcoded wait_for
    {
        if( (pinRead = digitalRead(_settings->getMotionFeatureGpio())) != _pinLastState)
        {
            if(pinRead == 1)
            {
                _noMotionTimer = time(nullptr) + (60 * _settings->getMotionDelayOff());
                _pinLastState = pinRead;
                emit motionLightsStateChange(this, _pinLastState);


            }
            else
            {
                if(_noMotionTimer <= time(nullptr))
                {
                    _pinLastState = pinRead;
                    emit motionLightsStateChange(this, _pinLastState);

                }
            }

        }
        else if(pinRead == 1)
        {
            _noMotionTimer = time(nullptr) + (60 * _settings->getMotionDelayOff());
        }
        else if(pinRead == 0 && _noMotionTimer > 0)
        {
            if(_noMotionTimer <= time(nullptr))
            {
                _pinLastState = pinRead;
                _noMotionTimer = 0;
                emit motionLightsStateChange(this, _pinLastState);

            }
        }

    }

    _running = false;
    emit motionLightsCompleted(this);

}

void MotionLightsFeature::stop()
{
    if(_running)
    {
        _running = false;
        _conditionVar.notify_all();
        _logger->logInfo("MotionLightsFeature Stopped.");
    }
}
