#include "showrainbow.h"

ShowRainbow::ShowRainbow(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}


void ShowRainbow::startShow()
{
    while(_endTime > time(nullptr))
    {
        for(uint16_t wheel = 0; wheel < 256; wheel ++)
        {
            for(uint16_t ledCount = 0; ledCount <_ledWrapper->getNumberLeds(); ledCount++)
                _ledWrapper->setPixelColor(ledCount,  _ledWrapper->Wheel((ledCount + wheel) & 255));

            _ledWrapper->show();
            if(_running == false || _endTime < time(nullptr))
                return;
            
            Ws2811Wrapper::waitMillSec(_wait);

        }
        Ws2811Wrapper::waitMillSec(_wait);
    }
}
