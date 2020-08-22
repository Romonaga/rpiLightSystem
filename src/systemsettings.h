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

    int getMatrix2121Wiring() const;

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
    int _matrix2121Wiring;

    DNRLogger* _logger;

    QMap<int, ChannelSettings*> _channels;

};

#endif
