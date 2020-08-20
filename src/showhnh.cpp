#include "showhnh.h"

ShowHnH::ShowHnH(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}


void ShowHnH::startShow()
{

    u_int32_t half = _ledWrapper->getNumberLeds() / 2;
    u_int32_t counter = 0;

    for(counter = 0; counter < half; counter++)
    {
        _ledWrapper->setPixelColor(counter, _color1);

    }

    for(; counter < _ledWrapper->getNumberLeds(); counter++)
    {
        _ledWrapper->setPixelColor(counter, _color2);

    }
    _ledWrapper->show();
}


