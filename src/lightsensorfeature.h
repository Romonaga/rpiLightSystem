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

#ifndef LightSensorFeature_H
#define LightSensorFeature_H

#include <QThread>
#include <QString>
#include <QSqlQuery>

#include <time.h>
#include  <condition_variable>

#include "systemsettings.h"
#include "dnrlogger.h"



class LightSensorFeature : public QThread
{
    Q_OBJECT

public:
    explicit LightSensorFeature(const QSqlQuery& qry);
    ~LightSensorFeature();
    void stop();
    uint32_t getFeaturePlayList() const;

private:
    int _pinLastState;

protected:
    SystemSettings* _settings;
    bool _running;
    DNRLogger* _logger;
    std::condition_variable _conditionVar;
    time_t _noMotionTimer;
    uint32_t _featurePlayList;
    short _gpioPin;



protected:
    void run();


signals:
    void lightSensorCompleted(LightSensorFeature*);
    void lightSensorStateChange(LightSensorFeature*, int);

};


#endif // LightSensorFeature_H

