#include "showcolor4.h"

ShowColor4::ShowColor4(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(settings,ledWrapper, lightShow, showParms)
{
    _wait = 200;
    _color1 = Ws2811Wrapper::Color(255, 0, 200);
    _color2 = Ws2811Wrapper::Color(255, 12, 15);
    _color3 = Ws2811Wrapper::Color(12, 0, 255);
    _color4 = Ws2811Wrapper::Color(0, 231, 255);

}


void ShowColor4::startShow()
{

    ws2811_return_t renderResults = WS2811_SUCCESS;
    ws2811_led_t colors[] = {_color1, _color2, _color3, _color4};

    u_int32_t forths = _ledWrapper->getNumberLeds() / 4;
    u_int32_t counter = 0;
    u_int32_t  currentDivision = forths;
    u_int8_t colorNumber = 1;

    ws2811_led_t currentColor = _color1;

    for(int count = 0; count < 25; count++)
    {
        _ledWrapper->clearLeds();

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
    _ledWrapper->clearLeds();

}


