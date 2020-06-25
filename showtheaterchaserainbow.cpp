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
        for (int colorWheel = 0; colorWheel < 256; colorWheel++)
        {

            for (int skippLoop = 0; skippLoop < 3; skippLoop++)
            {
                for (uint16_t onLoop = 0; onLoop < _ledWrapper->getNumberLeds(); onLoop += 3)
                    _ledWrapper->setPixelColor(_settings->getStripHeight(), onLoop + skippLoop, Ws2811Wrapper::Wheel( (onLoop + colorWheel) % 255));    //turn every third pixel on

                _ledWrapper->show();

                if(_running == false || _endTime < time(nullptr))
                    return;

              Ws2811Wrapper::waitMillSec(_wait);

              for (uint16_t offLoop = 0; offLoop < _ledWrapper->getNumberLeds(); offLoop+=3)
                  _ledWrapper->setPixelColor(_settings->getStripHeight(), offLoop + skippLoop, 0);        //turn every third pixel off


            }
        }
    }

}


/*
void ShowTheaterChaseRainBow::startShow()
{


    for (int j=0; j < _numLoops; j++)
    {
        if(_running == false)
            return;

        // cycle all 256 colors in the wheel
        for (int q=0; q < 3; q++)
        {
            for (u_int32_t i=0; i < _ledWrapper->getNumberLeds(); i=i+3)
            {
               _ledWrapper->setPixelColor(_settings->getStripHeight(), i+q, Ws2811Wrapper::Wheel( (i+j) % 255));    //turn every third pixel on
            }
            _ledWrapper->show();

          Ws2811Wrapper::waitMillSec(_wait);
          for (u_int32_t i=0; i < _ledWrapper->getNumberLeds(); i=i+3)
          {
            _ledWrapper->setPixelColor(_settings->getStripHeight(), i+q, 0);        //turn every third pixel off
          }
        }
    }

}


*/
