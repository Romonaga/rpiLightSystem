#include "showcyclon.h"

ShowCyclon::ShowCyclon(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(settings,ledWrapper, lightShow, showParms)
{
    _speed = 30;
    _width = 20;
    _color = Ws2811Wrapper::Color(29, 19, 100);
}



void ShowCyclon::startShow()
{

    _ledWrapper->clearLeds();

    for(int counter = 0; counter < 25; counter++)
    {
      // First slide the leds in one direction
      for(u_int32_t i = 0; i <= _ledWrapper->getNumberLeds() - _width; i++)
      {
        for(int j=0; j< _width; j++)
        {
            _ledWrapper->setPixelColor(_settings->getStripHeight(), i + j, _color);
        }

        _ledWrapper->show();

        // now that we've shown the leds, reset to black for next loop
        for(int j=0; j<5; j++)
        {
            _ledWrapper->setPixelColor(_settings->getStripHeight(), i + j, _ledWrapper->Color(0,0,0));
        }
        if(_running == false)
            return;
        Ws2811Wrapper::waitMillSec(_speed);
        _ledWrapper->clearLeds();
      }

      // Now go in the other direction.
      for(u_int32_t i = _ledWrapper->getNumberLeds() - _width; i == 0; i--)
      {
        for(int j=0; j< _width; j++)
        {
        _ledWrapper->setPixelColor(_settings->getStripHeight(), i + j, _color);
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
        Ws2811Wrapper::waitMillSec(_speed);
      }
    }
    _ledWrapper->clearLeds();
}


