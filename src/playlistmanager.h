#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <QObject>
#include <QString>
#include <QVector>

#include "ilightshow.h"
#include "systemsettings.h"
#include "dnrlogger.h"


class PlayListManager : public QObject
{
    Q_OBJECT
public:
    explicit PlayListManager(QObject *parent = nullptr);
    ~PlayListManager();
    bool savePlayList(const QString& name, int32_t userId, const QVector<ILightShow*>& shows);
    QString getPlayList(int32_t userId, int32_t playlistID);
    QString getPlayList(int32_t playlistID);
    bool deletePlayList(int32_t userId, int32_t playlistID);



private:
    SystemSettings* _settings;
    DNRLogger* _logger;


signals:

};


#endif // PLAYLISTMANAGER_H
