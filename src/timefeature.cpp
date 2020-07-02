#include "timefeature.h"
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <time.h>
#include <sstream>



TimeFeature::TimeFeature(const QSqlQuery& qry)
{
    _logger = DNRLogger::instance();
    _settings = SystemSettings::getInstance();
    _started = false;
     _featurePlayList = qry.value("featurePlayList").toUInt();

     _timeFeatureStart = qry.value("timeFeatureStart").toString();
     _timeFeatureEnd = qry.value("timeFeatureEnd").toString();

     _logger->logInfo("TimeFeature Init.");

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

    QTime startTime;
    QTime endTime;
    bool addDay = false;
    std::stringstream info;

    startTime = QTime::fromString(_timeFeatureStart,"HH:mm");
    endTime = QTime::fromString(_timeFeatureEnd,"HH:mm");

    if(startTime.hour() > endTime.hour()) addDay = true;

    QDateTime start = QDateTime(QDateTime::currentDateTime().date(), startTime);
    QDateTime end = (addDay == true) ? QDateTime(QDateTime::currentDateTime().date().addDays(1), endTime) : QDateTime(QDateTime::currentDateTime().date(), endTime);
    _startTime = start.toSecsSinceEpoch();
    _endTime = end.toSecsSinceEpoch();

    info << "Time Feature Start(" << start.toString().toStdString().c_str() << ") End(" << end.toString().toStdString().c_str() << ")";
    _logger->logInfo(info.str());

    _running = true;

    while(_conditionVar.wait_for(lck,std::chrono::milliseconds(300)) == std::cv_status::timeout && _running == true) //todo remove hardcoded wait_for
    {

        if(time(nullptr) >= _startTime &&  time(nullptr) < _endTime && _started == false)
        {
            _started = true;
            emit timeStateChange(this, 1);

        }
        else if(time(nullptr) >= _endTime && _started == true)
        {

            _started = false;
            start = QDateTime(QDateTime::currentDateTime().date(), startTime);
            end = (addDay == true) ? QDateTime(QDateTime::currentDateTime().date().addDays(1), endTime) : QDateTime(QDateTime::currentDateTime().date(), endTime);
            _startTime = start.toSecsSinceEpoch();
            _endTime = end.toSecsSinceEpoch();

            info.str("");
            info << "Time Feature Reset(" << start.toString().toStdString().c_str() << ") End(" << end.toString().toStdString().c_str() << ")";
            _logger->logInfo(info.str());

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


uint32_t TimeFeature::getFeaturePlayList() const
{
    return _featurePlayList;
}
