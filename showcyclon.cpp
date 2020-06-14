#include "showcyclon.h"

ShowCyclon::ShowCyclon(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{
    _width = 20;

}



void ShowCyclon::startShow()
{


    for(int counter = 0; counter < _numLoops; counter++)
    {
      // First slide the leds in one direction
      for(u_int32_t i = 0; i <= _ledWrapper->getNumberLeds() - _width; i++)
      {
        for(int j=0; j< _width; j++)
        {
            _ledWrapper->setPixelColor(_settings->getStripHeight(), i + j, _color1);
        }

        _ledWrapper->show();

        // now that we've shown the leds, reset to black for next loop
        for(int j=0; j<5; j++)
        {
            _ledWrapper->setPixelColor(_settings->getStripHeight(), i + j, _ledWrapper->Color(0,0,0));
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
        _ledWrapper->setPixelColor(_settings->getStripHeight(), i + j, _color1);
        }
        if(_running == false)
            return;
        _ledWrapper->show();
        for(int j=0; j< _width; j++)
        {
           _ledWrapper->setPixelColor(_settings->getStripHeight(), i + j, _ledWrapper->Color(0,0,0));
        }
        if(_running == false)
            return;
        Ws2811Wrapper::waitMillSec(_wait);
      }
    }

}


