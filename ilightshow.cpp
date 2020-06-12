#include "ilightshow.h"

ILightShow::ILightShow(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString& showParms) :
        _settings(settings), _ledWrapper(ledWrapper), _lightShow(lightShow), _showParms(showParms)
{
    _running = false;
    _logger = DNRLogger::instance();
}


void ILightShow::run()
{
    _running = true;
    startShow();
    _running = false;
    emit showComplete(this);
}

void ILightShow::stopShow()
{
    _running = false;
    wait();

}

LedLightShows ILightShow::getLightShow() const
{
    return _lightShow;
}



QString ILightShow::getShowName()
{
    const char* lightShowNames[] = {LIGHT_SHOWS(LIGHT_SHOWS_STRING)};
    return lightShowNames[_lightShow];
}
