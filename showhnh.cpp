#include "showhnh.h"

ShowHnH::ShowHnH(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(settings,ledWrapper, lightShow, showParms)
{

}


void ShowHnH::startShow()
{

    u_int32_t half = _ledWrapper->getNumberLeds() / 2;
    u_int32_t counter = 0;

    for(int count = 0; count < 25; count++)
    {

        for(counter = 0; counter < half; counter++)
        {
            _ledWrapper->setPixelColor(_settings->getStripHeight(), counter, _color1);
            _ledWrapper->show();

            Ws2811Wrapper::waitMillSec(_wait);
            if(_running == false)
                return;

        }

        for(; counter < _ledWrapper->getNumberLeds(); counter++)
        {
            _ledWrapper->setPixelColor(_settings->getStripHeight(), counter, _color2);
            _ledWrapper->show();
            Ws2811Wrapper::waitMillSec(_wait);
            if(_running == false)
                return;
        }
    }


}


