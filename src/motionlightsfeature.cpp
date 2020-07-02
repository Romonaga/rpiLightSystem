#include "motionlightsfeature.h"
#include <QDebug>
#include <time.h>
#include <wiringPi.h>



MotionLightsFeature::MotionLightsFeature(const QSqlQuery &qry)
{
    _logger = DNRLogger::instance();
    _settings = SystemSettings::getInstance();
    _noMotionTimer = 0;


    _motionDelayOff = qry.value("motionDelayOff").toUInt();
    _featurePlayList = qry.value("featurePlayList").toUInt();
    _gpioPin = qry.value("featureGpio").toUInt();
    _logger->logInfo("MotionLightsFeature Init.");

}

MotionLightsFeature::~MotionLightsFeature()
{
    stop();
    _logger->logInfo("MotionLightsFeature Stopped.");
}

void MotionLightsFeature::run()
{
    int pinRead;
    int pinLastState = 0;
    int resetLogCount = 1;

    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);

     _logger->logInfo("MotionLightsFeature run.");
    if(wiringPiSetupPhys() != 0)
    {
        _logger->logCritical("MotionLightsFeature WiringPiSetup Failed");
        return;
    }

    _running = true;
    pinMode(_gpioPin, INPUT);

    while(_conditionVar.wait_for(lck,std::chrono::milliseconds(300)) == std::cv_status::timeout && _running == true) //todo remove hardcoded wait_for
    {
        if( (pinRead = digitalRead(_gpioPin)) != pinLastState)
        {
            if(pinRead == 1)
            {
                _noMotionTimer = time(nullptr) + (60 * _motionDelayOff);
                pinLastState = pinRead;
                emit motionLightsStateChange(this, pinLastState);


            }
            else
            {
                if(_noMotionTimer <= time(nullptr))
                {
                    pinLastState = pinRead;
                    _logger->logInfo("MotionLightsFeature No Motion Detected.");
                    resetLogCount = 1;
                    emit motionLightsStateChange(this, pinLastState);

                }
            }

        }
        else if(pinRead == 1)
        {
            _noMotionTimer = time(nullptr) + (60 * _motionDelayOff);
            if(resetLogCount == 1)
            {
                _logger->logInfo("MotionLightsFeature Timer Reset Due To Motion.");
            }
            resetLogCount = (resetLogCount == 10) ? 1 : resetLogCount + 1;

        }
        else if(pinRead == 0 && _noMotionTimer > 0)
        {
            if(_noMotionTimer <= time(nullptr))
            {
                pinLastState = pinRead;
                _noMotionTimer = 0;
                resetLogCount = 1;
                emit motionLightsStateChange(this, pinLastState);

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

uint32_t MotionLightsFeature::getFeaturePlayList() const
{
    return _featurePlayList;
}
