#include "showtwinkle.h"


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
    foreach (unsigned int led, _twinks.keys())
    {
       _ledWrapper->setPixelColor(_settings->getStripHeight(), led, _twinks[led]);
       _ledWrapper->show();
       Ws2811Wrapper::waitMillSec(_wait);
    }

    _twinks.clear();
}

void ShowTwinkle::startShow()
{

    int twinkLed;

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
                        _ledWrapper->setPixelColor(_settings->getStripHeight(), twinkLed,  Ws2811Wrapper::Wheel(genRand(1, _color1)));
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

