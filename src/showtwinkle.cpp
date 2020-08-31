#include "showtwinkle.h"
#include <sstream>
#include <QDebug>

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

    QPair<unsigned int, unsigned int> pair;
    foreach(pair,_twinks)
    {
        _ledWrapper->setPixelColor(pair.first, pair.second);
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

    unsigned int twinkLed;

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


    bool found = false;
    while(_endTime > time(nullptr))
    {

        found = false;
        twinkChance =  genRand(1, _ledWrapper->getNumberLeds() * .1);

        for (unsigned int inner = 0; inner < _ledWrapper->getNumberLeds(); inner++)
        {

            if( twinkChance <= genRand(1, _ledWrapper->getNumberLeds()) - 1)
            {
                twinkLed = genRand(1, _ledWrapper->getNumberLeds() - 1);

                QPair<unsigned int, unsigned int> pair;
                foreach(pair,_twinks)
                {
                   if(pair.first == twinkLed)
                   {
                       found = true;
                       break;
                   }

                }

                if(found == false)
                {
                    ws2811_led_t getcolor = _ledWrapper->getPixelColor(twinkLed);
                    if(getcolor != 0)
                    {
                        _twinks.append(QPair<unsigned int, unsigned int>(twinkLed, getcolor));
                        _ledWrapper->setPixelColor(twinkLed, genRand(min, max));
                        _ledWrapper->show();
                        
                    }
                }
            }
        }

        resetLeds();

        if(_running == false)
            return;

    }
}

