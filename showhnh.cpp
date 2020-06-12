#include "showhnh.h"

ShowHnH::ShowHnH(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(settings,ledWrapper, lightShow, showParms)
{
    _wait = 200;
    _color1 = Ws2811Wrapper::Color(125, 19, 88);
    _color2 = Ws2811Wrapper::Color(27, 99, 155);


}


void ShowHnH::startShow()
{

    u_int32_t half = _ledWrapper->getNumberLeds() / 2;
    u_int32_t counter = 0;

    for(int count = 0; count < 25; count++)
    {
        _ledWrapper->clearLeds();
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

    _ledWrapper->clearLeds();

}


