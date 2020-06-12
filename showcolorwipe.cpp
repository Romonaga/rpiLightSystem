#include "showcolorwipe.h"

ShowColorWipe::ShowColorWipe(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(settings,ledWrapper, lightShow, showParms)
{
    _color = _ledWrapper->Color(58, 12 , 170);
    _wait = 200;

}



void ShowColorWipe::startShow()
{

    _ledWrapper->clearLeds();
    for(int counter = 0; counter < 25; counter++)
    {

        for(uint32_t i=0; i < _ledWrapper->getNumberLeds(); i++)
        {
          _ledWrapper->setPixelColor(_settings->getStripHeight(), i, _color);
          Ws2811Wrapper::waitMillSec(_wait);
          _ledWrapper->show();
          if(_running == false)
              return;
          Ws2811Wrapper::waitMillSec(_wait);

        }
   }
   _ledWrapper->clearLeds();
}


