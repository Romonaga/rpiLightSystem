#include "showcolor.h"

ShowColor::ShowColor(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{
   
}



void ShowColor::startShow()
{


    _ledWrapper->setPixelColor(_color1);

    _ledWrapper->show();

}

