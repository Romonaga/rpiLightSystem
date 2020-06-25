#include "showrainbowcycle.h"

ShowRainbowCycle::ShowRainbowCycle(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}



void ShowRainbowCycle::startShow()
{

    u_int32_t i, j;

    for(j=0; j < _numLoops ; j++)
    {
        if(_running == false)
            return;

        // 5 cycles of all colors on wheel
        for(i=0; i < _ledWrapper->getNumberLeds(); i++)
           _ledWrapper->setPixelColor(_settings->getStripHeight(), i, Ws2811Wrapper::Wheel(((i * 256 / _ledWrapper->getNumberLeds()) + j) & 255));

        _ledWrapper->show();
        if(_running == false)
            return;
        Ws2811Wrapper::waitMillSec(_wait);
    }

}

