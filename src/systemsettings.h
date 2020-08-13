#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

#include <QString>
#include <QMap>
#include <dnrlogger.h>
#include "channelsettings.h"

class SystemSettings
{

public:
    static SystemSettings* getInstance();

    bool loadSettings();
    bool settingsExists();

    QString getDBServer() const;
    void setServer(const QString &server);

    QString getDataBase() const;
    void setDataBase(const QString &dataBase);

    QString getDBUser() const;
    void setUser(const QString &user);

    QString getDBPwd() const;
    void setPwd(const QString &pwd);



    bool loadSystemSettings();
    
    QString getSystemName() const;
    QString getHostName() const;
    QString getMqttBroker() const;
    bool getDbgLog() const;
    bool getLogShows() const;
    int getSystemId() const;
    bool getTwitchSupport() const;
    int getMqttRetries() const;
    int getMqttRetryDelay() const;
    QString getMqttTwitchQueue() const;


    QMap<int, ChannelSettings *> getChannels() const;

    QString getUserArtDirectory() const;

private:
    SystemSettings();
    static SystemSettings* _instance;
    QString _server;
    QString _dataBase;
    QString _user;
    QString _pwd;
    QString  _mmConfFile;
    QString _hostName;
    QString _mqttBroker;
    QString _mqttTwitchQueue;

    //System Setings From DB
    QString _systemName;
    int _systemId;
    bool _logShows;
    bool _twitchSupport;
    int _mqttRetries;
    int _mqttRetryDelay;
    bool _dbgLog;
    QString _userArtDirectory;

    DNRLogger* _logger;

    QMap<int, ChannelSettings*> _channels;

};

#endif
