#include "showrainbow.h"

ShowRainbow::ShowRainbow(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(settings,ledWrapper, lightShow, showParms)
{
    _wait = 200;

}



void ShowRainbow::startShow()
{

    u_int32_t i, j;

    _ledWrapper->clearLeds();

    for(j=0; j < 256; j++)
    {
      if(_running == false)
          return;

      for(i=0; i <_ledWrapper->getNumberLeds(); i++)
      {
        _ledWrapper->setPixelColor(_settings->getStripHeight(), i,  Ws2811Wrapper::Wheel((i+j) & 255));
      }
      _ledWrapper->show();
      if(_running == false)
          return;
      Ws2811Wrapper::waitMillSec(_wait);

    }
    _ledWrapper->clearLeds();

}


