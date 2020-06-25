#include "showPulse.h"

ShowPulse::ShowPulse(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}



void ShowPulse::startShow()
{

    while(_endTime > time(nullptr))
    {
       if(_running == false) return;

       for(uint16_t ledCount = 0; ledCount <_ledWrapper->getNumberLeds(); ledCount++)
       {
          for(int brightLoop = _brightness; brightLoop >= 1; brightLoop--)
          {
              _ledWrapper->setBrightness(brightLoop);
              _ledWrapper->show();
              Ws2811Wrapper::waitMillSec(10);
          }

          _ledWrapper->setBrightness(_brightness);
          _ledWrapper->show();
       }

       Ws2811Wrapper::waitMillSec(_wait);
    }
}


