#include "showtwinkle.h"
#include <sstream>

ShowTwinkle::ShowTwinkle(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}


ShowTwinkle::~ShowTwinkle()
{
    resetLeds();
}

void ShowTwinkle::resetLeds()
{
    unsigned int removeIndex = 0;
    unsigned int index = 0;
    ws2811_led_t color = 0;

    while(_twinks.count( ) > 0)
    {
        index = 0;
        removeIndex = genRand(1, _twinks.count());
        foreach (ws2811_led_t led, _twinks.keys())
        {
          index++;
          if(index == removeIndex)
          {
              color = _twinks.value(led);
              _ledWrapper->setPixelColor(_settings->getStripHeight(), led, color);
              _twinks.remove(led);
              Ws2811Wrapper::waitMillSec((int)_wait / 2);
              break;
          }

        }
    }

}

void ShowTwinkle::startShow()
{
    ws2811_led_t min;
    ws2811_led_t max;

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


    int twinkChance =  genRand(1, _ledWrapper->getNumberLeds() * .5);
    while(_endTime > time(nullptr))
    {

        resetLeds();

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
        Ws2811Wrapper::waitMillSec(_wait);
    }
}

