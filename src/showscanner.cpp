#include "showscanner.h"

ShowScanner::ShowScanner(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{
   
}



void ShowScanner::startShow()
{

    u_int32_t index = 0;
    u_int32_t totalSteps = (_ledWrapper->getNumberLeds() - 1) * 2;

    while(_endTime > time(nullptr))
    {
        while(index <= totalSteps)
        {
            for (u_int32_t led = 0; led < _ledWrapper->getNumberLeds(); led++)
            {
                if (led == index) // first half of the scan
                {
                    _ledWrapper->setPixelColor(1, led, _color1);

                }
                else if (led == (totalSteps - led)) // The return trip.
                {
                    _ledWrapper->setPixelColor(1, led, _color1);
                }
                else  // fade to black
                {
                    _ledWrapper->setPixelColor(1,led, _ledWrapper->DimColor(_ledWrapper->getPixelColor(1, led)));
                }
                _ledWrapper->show();

                if(_running == false || _endTime < time(nullptr))
                    return;

                Ws2811Wrapper::waitMillSec(_wait);

                index++;
            }
        }
        _ledWrapper->clearLeds();
    }

}

