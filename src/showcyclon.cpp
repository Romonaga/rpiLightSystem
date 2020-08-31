#include "showcyclon.h"

ShowCyclon::ShowCyclon(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}



void ShowCyclon::startShow()
{


    while(_endTime > time(nullptr))
    {
      // First slide the leds in one direction
      for(u_int32_t i = 0; i <= _ledWrapper->getNumberLeds() - _width; i++)
      {
        for(int j=0; j< _width; j++)
        {
            _ledWrapper->setPixelColor(i + j, _color1);
        }

        _ledWrapper->show();

        // now that we've shown the leds, reset to black for next loop
        for(int j=0; j < _width; j++)
        {
            _ledWrapper->setPixelColor(i + j, _ledWrapper->color(0,0,0));
        }
        if(_running == false)
            return;
        Ws2811Wrapper::waitMillSec(_wait);
        _ledWrapper->clearLeds();
      }

      // Now go in the other direction.
      for(u_int32_t i = _ledWrapper->getNumberLeds() - _width; i == 0; i--)
      {
        for(int j=0; j< _width; j++)
        {
        _ledWrapper->setPixelColor(i + j, _color1);
        }
        if(_running == false)
            return;
        _ledWrapper->show();
        for(int j=0; j< _width; j++)
        {
           _ledWrapper->setPixelColor(i + j, _ledWrapper->color(0,0,0));
        }
        if(_running == false)
            return;
        Ws2811Wrapper::waitMillSec(_wait);
      }
    }

}


