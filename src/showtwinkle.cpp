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
    foreach(pair,_twinks2)
    {
        _ledWrapper->setPixelColor(_settings->getStripHeight(), pair.first, pair.second);
        _ledWrapper->show();
        Ws2811Wrapper::waitMillSec(genRand(1, _wait));

    }
     _twinks2.clear();
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
        twinkChance =  genRand(1, _ledWrapper->getNumberLeds() * .3);

        for (unsigned int inner = 0; inner < _ledWrapper->getNumberLeds(); inner++)
        {

            if( twinkChance <= genRand(1, _ledWrapper->getNumberLeds()) - 1)
            {
                twinkLed = genRand(1, _ledWrapper->getNumberLeds() - 1);

                QPair<unsigned int, unsigned int> pair;
                foreach(pair,_twinks2)
                {
                   if(pair.first == twinkLed)
                   {
                       found = true;
                       break;
                   }

                }

                if(found == false)
                {
                    ws2811_led_t getColor = _ledWrapper->getPixelColor(_settings->getStripHeight(), twinkLed);
                    if(getColor != 0)
                    {
                        _twinks2.append(QPair<unsigned int, unsigned int>(twinkLed, getColor));
                        _ledWrapper->setPixelColor(_settings->getStripHeight(), twinkLed, genRand(min, max));
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

