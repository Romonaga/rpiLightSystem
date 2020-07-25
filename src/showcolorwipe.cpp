#include "showcolorwipe.h"

ShowColorWipe::ShowColorWipe(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}



void ShowColorWipe::startShow()
{

     while(_endTime > time(nullptr))
    {

         for(u_int32_t row = 0; row <= _settings->getStripRows(); row++)
         {
             for(u_int32_t column = 0; column <= _settings->getStripColumns(); column++)
             {
                 _ledWrapper->setPixelColor(row, column, _color1);
                 Ws2811Wrapper::waitMillSec(_wait);
                 _ledWrapper->show();
                 if(_running == false)
                     return;
                 Ws2811Wrapper::waitMillSec(_wait);
             }
         }


        _ledWrapper->clearLeds();
   }

}


