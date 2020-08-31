#include "showtheaterchaserainbow.h"

ShowTheaterChaseRainBow::ShowTheaterChaseRainBow(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}


void ShowTheaterChaseRainBow::startShow()
{

    while(_endTime > time(nullptr))
    {
        // cycle all 256 colors in the wheel
        for (int colorwheel = 0; colorwheel < 256; colorwheel++)
        {

            for (int skippLoop = 0; skippLoop < 3; skippLoop++)
            {
                for (uint16_t onLoop = 0; onLoop < _ledWrapper->getNumberLeds(); onLoop += 3)
                    _ledWrapper->setPixelColor(onLoop + skippLoop, _ledWrapper->wheel( (onLoop + colorwheel) % 255));    //turn every third pixel on

                _ledWrapper->show();

                if(_running == false || _endTime < time(nullptr))
                    return;

              Ws2811Wrapper::waitMillSec(_wait);

              for (uint16_t offLoop = 0; offLoop < _ledWrapper->getNumberLeds(); offLoop+=3)
                  _ledWrapper->setPixelColor(offLoop + skippLoop, 0);        //turn every third pixel off


            }
        }
    }

}


