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


#ifndef CHANNELSETTINGS_H
#define CHANNELSETTINGS_H

#include <QSqlQuery>


class ChannelSettings
{
public:
    ChannelSettings(QSqlQuery qry);

    int stripType() const;
    int systemId() const;
    uint16_t stripColumns() const;
    uint16_t stripRows() const;
    short dma() const;
    short gpio() const;
    int brightness() const;
    int matrixdirection() const;
    double gamma() const;


    int channelId() const;

    void setBrightness(int brightness);

    int matrix2121Wiring() const;

private:
    int _channelId;
    int _stripType;
    int _systemId;
    uint16_t _stripColumns;
    uint16_t _stripRows;
    short _dma;
    short _gpio;
    int _brightness;
    int _matrixdirection;
    double _gamma;
    int _matrix2121Wiring;

};

#endif // CHANNELSETTINGS_H
