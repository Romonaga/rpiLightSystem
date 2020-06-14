#include "showcolor4.h"

ShowColor4::ShowColor4(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}


void ShowColor4::startShow()
{

    ws2811_return_t renderResults = WS2811_SUCCESS;
    ws2811_led_t colors[] = {_color1, _color2, _color3, _color4};

    u_int32_t forths = _ledWrapper->getNumberLeds() / 4;
    u_int32_t counter = 0;


    for(int count = 0; count < _numLoops; count++)
    {
        _ledWrapper->clearLeds();
        u_int32_t  currentDivision = forths;
        u_int8_t colorNumber = 1;

        ws2811_led_t currentColor = _color1;

        for(counter = 0; counter < _ledWrapper->getNumberLeds(); counter++)
        {
            if(false == _running)
                return;

            if(counter >= currentDivision)
            {
                ++colorNumber;
                currentDivision = forths * colorNumber;
                currentColor = colors[colorNumber];
            }

            _ledWrapper->setPixelColor(_settings->getStripHeight(), counter, currentColor);
            if( (renderResults = _ledWrapper->show()) != WS2811_SUCCESS)
            {
                _logger->logWarning("ShowColor4 Render Failed");
                return;
            }
            Ws2811Wrapper::waitMillSec(_wait);

        }
    }
  

}


