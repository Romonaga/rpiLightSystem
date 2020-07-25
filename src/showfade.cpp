#include "showfade.h"

ShowFade::ShowFade(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}



void ShowFade::startShow()
{

    while(_endTime > time(nullptr))
    {
       if(_running == false) return;

       for(int counter = 0; counter < _width; counter ++)
       {
           for(uint16_t ledCount = 0; ledCount <_ledWrapper->getNumberLeds(); ledCount++)
           {

               ws2811_led_t color =  _ledWrapper->getPixelColor(1, ledCount);
               _ledWrapper->setPixelColor(1, ledCount,  _ledWrapper->DimColor(color));

           }
           _ledWrapper->show();
           Ws2811Wrapper::waitMillSec(_wait);
       }

       if(_running == false)
           return;


       for(int counter = 0; counter < _width; counter ++)
       {
           for(uint16_t ledCount = 0; ledCount <_ledWrapper->getNumberLeds(); ledCount++)
           {
                ws2811_led_t color =  _ledWrapper->getPixelColor(1, ledCount);

                _ledWrapper->setPixelColor(ledCount,  _ledWrapper->BrightenColor(color));

           }
           _ledWrapper->show();
           Ws2811Wrapper::waitMillSec(_wait);
        }
    }
}


