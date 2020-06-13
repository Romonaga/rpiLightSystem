#include "showcolor3r.h"

ShowColor3R::ShowColor3R(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(settings,ledWrapper, lightShow, showParms)
{

}


void ShowColor3R::startShow()
{

    uint32_t colors[] = {_color1, _color2, _color3};

    if (_running == false) return;


    for(int counter = 0 ; counter <= 3; counter++)
    {
      u_int32_t third = _ledWrapper->getNumberLeds() / 3;
      u_int32_t currentDivision = (third * 2);
      int colorNumber = 1;

      uint32_t currentColor = _color1;


      for (u_int32_t counter = (_ledWrapper->getNumberLeds() - 1); counter > 0; counter--)
      {

        if (counter <= currentDivision)
        {
          ++colorNumber;
          currentDivision = (currentDivision - third);
          currentColor = colors[colorNumber - 1];
        }


        _ledWrapper->setPixelColor(_settings->getStripHeight(), counter, currentColor);
        _ledWrapper->show();

        if (_running == false) return;

         Ws2811Wrapper::waitMillSec(_wait);
      }

    }


}


