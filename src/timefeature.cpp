#include "timefeature.h"
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <time.h>




TimeFeature::TimeFeature()
{
    _logger = DNRLogger::instance();
    _settings = SystemSettings::getInstance();
    _started = false;
}

TimeFeature::~TimeFeature()
{
    stop();
    _logger->logInfo("TimeFeature Shutting Down.");
}

void TimeFeature::run()
{
    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);

     _logger->logInfo("TimeFeature run.");

    QTime startTime;
    QTime endTime;
    bool addDay = false;

    startTime = QTime::fromString(_settings->getTimeFeatureStart(),"HH:mm");
    endTime = QTime::fromString(_settings->getTimeFeatureEnd(),"HH:mm");

    if(startTime.hour() > endTime.hour()) addDay = true;

    qDebug() << "QT Start: " << startTime << " End: " << endTime;

    _running = true;

    while(_conditionVar.wait_for(lck,std::chrono::milliseconds(300)) == std::cv_status::timeout && _running == true) //todo remove hardcoded wait_for
    {

        _startTime = QDateTime(QDateTime::currentDateTime().date(), startTime).toSecsSinceEpoch();


        _endTime = QDateTime(QDateTime::currentDateTime().date(), endTime).toSecsSinceEpoch();       
        _endTime  = (addDay == true) ? QDateTime(QDateTime::currentDateTime().date(), endTime).addDays(1).toSecsSinceEpoch() : QDateTime(QDateTime::currentDateTime().date(), endTime).toSecsSinceEpoch();

        if(time(nullptr) >= _startTime &&  time(nullptr) < _endTime && _started == false)
        {
            _started = true;
            emit timeStateChange(this, 1);

        }
        else if(time(nullptr) >= _endTime && _started == true)
        {

            _started = false;
            emit timeStateChange(this, 0);
        }


    }

    _running = false;
    emit timeCompleted(this);

}

void TimeFeature::stop()
{
    if(_running)
    {
        _running = false;
        _conditionVar.notify_all();
        _logger->logInfo("TimeFeature Stopped.");
    }
}
