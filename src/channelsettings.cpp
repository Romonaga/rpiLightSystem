#include "channelsettings.h"
#include <QVariant>

ChannelSettings::ChannelSettings(QSqlQuery qry)
{
    _systemId = qry.value("lightSystemId").toUInt();
    _stripType = qry.value("stripType").toUInt();
    _stripColumns = qry.value("stripColumns").toUInt();
    _stripRows = qry.value("stripRows").toUInt();
    _dma = qry.value("dma").toInt();
    _gpio = qry.value("gpio").toInt();
    _brightness = qry.value("brightness").toInt();
    _gamma = qry.value("gamma").toDouble();
    _matrixdirection = qry.value("matrixDirection").toInt();
    _channelId = qry.value("channelId").toInt() - 1;
    _matrix2121Wiring = qry.value("matrix2121Wiring").toInt();

}

int ChannelSettings::stripType() const
{
    return _stripType;
}



uint16_t ChannelSettings::stripColumns() const
{
    return _stripColumns;
}

uint16_t ChannelSettings::stripRows() const
{
    return _stripRows;
}

short ChannelSettings::dma() const
{
    return _dma;
}

short ChannelSettings::gpio() const
{
    return _gpio;
}

int ChannelSettings::brightness() const
{
    return _brightness;
}

int ChannelSettings::matrixdirection() const
{
    return _matrixdirection;
}

double ChannelSettings::gamma() const
{
    return _gamma;
}

int ChannelSettings::channelId() const
{
    return _channelId;
}

void ChannelSettings::setBrightness(int brightness)
{
    _brightness = brightness;
}

int ChannelSettings::matrix2121Wiring() const
{
    return _matrix2121Wiring;
}
