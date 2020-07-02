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

