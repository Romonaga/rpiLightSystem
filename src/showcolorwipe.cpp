#include "showcolorwipe.h"

ShowColorWipe::ShowColorWipe(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}



void ShowColorWipe::startShow()
{

     while(_endTime > time(nullptr))
    {

         for(u_int32_t counter = 0; counter < _ledWrapper->getNumberLeds(); counter++)
         {
                 _ledWrapper->setPixelColor(counter, _color1);
                 Ws2811Wrapper::waitMillSec(_wait);
                 _ledWrapper->show();
                 if(_running == false)
                     return;
                 Ws2811Wrapper::waitMillSec(_wait);

         }


        _ledWrapper->clearLeds();
   }

}


