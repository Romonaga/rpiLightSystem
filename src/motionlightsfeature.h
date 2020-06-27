#ifndef MotionLightFeature_H
#define MotionLightFeature_H

#include <QThread>
#include <QString>
#include <time.h>
#include  <condition_variable>

#include "systemsettings.h"
#include "dnrlogger.h"



class MotionLightsFeature : public QThread
{
    Q_OBJECT

public:
    explicit MotionLightsFeature();
    void stop();

private:
    int _pinLastState;
    SystemSettings* _settings;
    bool _running;
    DNRLogger* _logger;
    std::condition_variable _conditionVar;
    time_t _noMotionTimer;


protected:
    void run();


signals:
    void motionLightsCompleted(MotionLightsFeature*);
    void motionLightsStateChange(MotionLightsFeature*, int);

};


#endif // MotionLightFeature_H

