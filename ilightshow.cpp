#include "ilightshow.h"

ILightShow::ILightShow(SystemSettings* settings, Ws2811Wrapper* ledWrapper, QObject *parent ) :
        QObject(parent), _settings(settings), _ledWrapper(ledWrapper)
{
    _running = false;
    _lightShow = Nope;
}

LedLightShows ILightShow::lightShow() const
{
    return _lightShow;
}

void ILightShow::setLightShow(const LedLightShows &lightShow)
{
    _lightShow = lightShow;
}


QString ILightShow::getShowName()
{
    const char* lightShowNames[] = {LIGHT_SHOWS(LIGHT_SHOWS_STRING)};
    return lightShowNames[_lightShow];
}
