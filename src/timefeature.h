// Copyright (C) 2020 Robert Winslow <robertwinslow@comcast.net>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//

#ifndef TimeFeature_H
#define TimeFeature_H

#include <QThread>
#include <QString>
#include <QSqlQuery>

#include <time.h>
#include  <condition_variable>

#include "systemsettings.h"
#include "dnrlogger.h"



class TimeFeature : public QThread
{
    Q_OBJECT

public:
    explicit TimeFeature(const QSqlQuery& qry);
    ~TimeFeature();
    void stop();

    uint32_t getFeaturePlayList() const;

private:
    SystemSettings* _settings;
    bool _running;
    DNRLogger* _logger;
    std::condition_variable _conditionVar;
    QString _timeFeatureStart;
    QString _timeFeatureEnd;

    time_t _startTime;
    time_t _endTime;
    bool _started;
    uint32_t _featurePlayList;



protected:
    void run();


signals:
    void timeCompleted(TimeFeature*);
    void timeStateChange(TimeFeature*, int);

};


#endif // TimeFeature_H

