#include "showcolor.h"

ShowColor::ShowColor(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{
   
}



void ShowColor::startShow()
{


    for (u_int32_t i = 0; i < _ledWrapper->getNumberLeds(); i++)
    {
      _ledWrapper->setPixelColor(_settings->getStripHeight(), i, _color1);
    }

    _ledWrapper->show();

}


