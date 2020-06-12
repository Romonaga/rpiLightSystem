#include "showblink.h"

ShowBlink::ShowBlink(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(settings,ledWrapper, lightShow, showParms)
{
    _numBlinks = 5;
    _wait = 100;
    _brightness = 60;
}



void ShowBlink::startShow()
{

    for (int counter = 0; counter < _numBlinks; counter++)
    {
       if(_running == false) return;
      _ledWrapper->setBrightness(1);
      _ledWrapper->show();
       Ws2811Wrapper::waitMillSec(_wait);
      _ledWrapper->setBrightness(_brightness);
      _ledWrapper->show();
       Ws2811Wrapper::waitMillSec(_wait);
    }
}


