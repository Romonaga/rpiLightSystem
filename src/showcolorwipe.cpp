#include "showcolorwipe.h"

ShowColorWipe::ShowColorWipe(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}



void ShowColorWipe::startShow()
{

     while(_endTime > time(nullptr))
    {

        for(uint32_t i=0; i < _ledWrapper->getNumberLeds(); i++)
        {
          _ledWrapper->setPixelColor(_settings->getStripHeight(), i, _color1);
          Ws2811Wrapper::waitMillSec(_wait);
          _ledWrapper->show();
          if(_running == false)
              return;
          Ws2811Wrapper::waitMillSec(_wait);

        }
        _ledWrapper->clearLeds();
   }

}


