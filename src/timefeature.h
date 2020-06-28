#ifndef TimeFeature_H
#define TimeFeature_H

#include <QThread>
#include <QString>
#include <time.h>
#include  <condition_variable>

#include "systemsettings.h"
#include "dnrlogger.h"



class TimeFeature : public QThread
{
    Q_OBJECT

public:
    explicit TimeFeature();
    ~TimeFeature();
    void stop();


private:
    SystemSettings* _settings;
    bool _running;
    DNRLogger* _logger;
    std::condition_variable _conditionVar;
    time_t _startTime;
    time_t _endTime;
    bool _started;



protected:
    void run();


signals:
    void timeCompleted(TimeFeature*);
    void timeStateChange(TimeFeature*, int);

};


#endif // TimeFeature_H

