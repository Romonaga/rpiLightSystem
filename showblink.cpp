#include "showblink.h"

ShowBlink::ShowBlink(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}



void ShowBlink::startShow()
{

    for (int counter = 0; counter < _numLoops; counter++)
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


