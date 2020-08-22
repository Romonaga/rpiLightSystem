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


#ifndef MotionLightFeature_H
#define MotionLightFeature_H

#include <QThread>
#include <QString>
#include <QSqlQuery>

#include <time.h>
#include  <condition_variable>

#include "systemsettings.h"
#include "dnrlogger.h"



class MotionLightsFeature : public QThread
{
    Q_OBJECT

public:
    explicit MotionLightsFeature(const QSqlQuery& qry);
    ~MotionLightsFeature();
    void stop();

    uint32_t getFeaturePlayList() const;

private:
    SystemSettings* _settings;
    bool _running;
    DNRLogger* _logger;
    std::condition_variable _conditionVar;
    time_t _noMotionTimer;
    uint32_t _motionDelayOff;
    uint32_t motionDelayOff;
    uint32_t _featurePlayList;
    short _gpioPin;



protected:
    void run();


signals:
    void motionLightsCompleted(MotionLightsFeature*);
    void motionLightsStateChange(MotionLightsFeature*, int);

};


#endif // MotionLightFeature_H

