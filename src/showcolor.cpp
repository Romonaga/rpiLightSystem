#include "showcolor.h"

ShowColor::ShowColor(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{
   
}



void ShowColor::startShow()
{

    for(u_int32_t row = 1; row <= _ledWrapper->getHight(); row++)
    {
        for (u_int32_t led = 0; led < _ledWrapper->getWidth(); led++)
        {
          _ledWrapper->setPixelColor(row, led, _color1);
        }
    }

    _ledWrapper->show();

}

