#ifndef LightSensorFeature_H
#define LightSensorFeature_H

#include <QThread>
#include <QString>
#include <time.h>
#include  <condition_variable>

#include "systemsettings.h"
#include "dnrlogger.h"



class LightSensorFeature : public QThread
{
    Q_OBJECT

public:
    explicit LightSensorFeature();
    void stop();

private:
    int _pinLastState;

protected:
    SystemSettings* _settings;
    bool _running;
    DNRLogger* _logger;
    std::condition_variable _conditionVar;
    time_t _noMotionTimer;


protected:
    void run();


signals:
    void lightSensorCompleted(LightSensorFeature*);
    void lightSensorStateChange(LightSensorFeature*, int);

};


#endif // LightSensorFeature_H

