#include "showrainbowcycle.h"

ShowRainbowCycle::ShowRainbowCycle(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}

void ShowRainbowCycle::startShow()
{
    while(_endTime > time(nullptr))
    {
        // 5 cycles of all colors on wheel
        for(uint16_t cycle =0; cycle < 256 * 5; cycle++)
        {
            if(_running == false || _endTime < time(nullptr))
                return ;
            for(uint16_t row = 0; row < _settings->getChannels()[_channelId]->stripRows(); row ++)
            {
                for(uint16_t col = 0; col < _settings->getChannels()[_channelId]->stripColumns(); col ++)
                {
                    _ledWrapper->setPixelColor(row, col, _ledWrapper->Wheel((((row * col) * 256 / _ledWrapper->getNumberLeds()) + cycle) & 255));

                }

            }

            //for(uint16_t led =0; led < _ledWrapper->getNumberLeds() ; led++)
              // _ledWrapper->setPixelColor(led, _ledWrapper->Wheel(((led * 256 / _ledWrapper->getNumberLeds()) + cycle) & 255));

             _ledWrapper->show();

            Ws2811Wrapper::waitMillSec(_wait);
        }
    }

}
