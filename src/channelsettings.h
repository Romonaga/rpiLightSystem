// jpge.h - C++ class for JPEG compression.
// Public domain, LumaWin <robertwinslow@comcast.net>


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
