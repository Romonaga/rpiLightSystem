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
    //Keeping as it has a good example of extracting JSON to string.
  //  bool savePlayList(const QString& name, int32_t userId, const QVector<ILightShow*>& shows);
    QString getPlayList(QJsonObject playList);
    QString getArtShow(QJsonObject artShow);
    QString getPlayList(int32_t playlistID);

    //QString getArtShow(int32_t artShowID);



private:
    SystemSettings* _settings;
    DNRLogger* _logger;


signals:

};


#endif // PLAYLISTMANAGER_H
