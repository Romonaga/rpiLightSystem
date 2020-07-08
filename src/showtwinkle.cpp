#include "showtwinkle.h"
#include <sstream>

ShowTwinkle::ShowTwinkle(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}


ShowTwinkle::~ShowTwinkle()
{
    _logger->logInfo("~ShowTwinkle");
    resetLeds();
}

void ShowTwinkle::resetLeds()
{
    unsigned int removeIndex = 0;
    unsigned int index = 0;
/*
    while(_twinks.count( ) > 0)
    {
        index = 0;
        removeIndex = genRand(1, _twinks.count());
        foreach (ws2811_led_t led, _twinks.keys())
        {
          index++;
          if(index == removeIndex)
          {

              _ledWrapper->setPixelColor(_settings->getStripHeight(), led, _twinks[led]);
              _twinks.remove(led);
              Ws2811Wrapper::waitMillSec(genRand(1, _wait));
              break;
          }

        }
    }
*/
     foreach (ws2811_led_t led, _twinks.keys())
     {
         _ledWrapper->setPixelColor(_settings->getStripHeight(), led, _twinks[led]);
         _ledWrapper->show();
          Ws2811Wrapper::waitMillSec(genRand(1, _wait));
     }

     _twinks.clear();
}

void ShowTwinkle::startShow()
{
    ws2811_led_t min;
    ws2811_led_t max;
    int twinkChance  = 0;

    int twinkLed;

    if(_color1 > _color2)
    {
        max = _color1;
        min = _color2;
    }
    else
    {
        min = _color1;
        max = _color2;
    }


    while(_endTime > time(nullptr))
    {

        resetLeds();

        twinkChance =  genRand(1, _ledWrapper->getNumberLeds() * .2);

        for (unsigned int inner = 0; inner < _ledWrapper->getNumberLeds(); inner++)
        {

            if(twinkChance > genRand(1, _ledWrapper->getNumberLeds()) - 1)
            {
                twinkLed = genRand(1, _ledWrapper->getNumberLeds()) - 1;
                if(_twinks.contains(twinkLed) == false)
                {
                    ws2811_led_t getColor = _ledWrapper->getPixelColor(_settings->getStripHeight(), twinkLed);
                    if(getColor != 0)
                    {
                        _twinks.insert(twinkLed, getColor);
                        _ledWrapper->setPixelColor(_settings->getStripHeight(), twinkLed, genRand(min, max));
                        _ledWrapper->show();
                        
                    }
                }
            }
        }

        if(_running == false)
            return;
       // Ws2811Wrapper::waitMillSec(_wait);
    }
}

