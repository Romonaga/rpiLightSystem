#include "showcolor3.h"

ShowColor3::ShowColor3(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(settings,ledWrapper, lightShow, showParms)
{
    _wait = 50;
    _color1 = Ws2811Wrapper::Color(255, 0, 200);
    _color2 = Ws2811Wrapper::Color(255, 12, 15);
    _color3 = Ws2811Wrapper::Color(12, 0, 255);

}


void ShowColor3::startShow()
{

    ws2811_return_t renderResults = WS2811_SUCCESS;
    ws2811_led_t colors[] = {_color1, _color2, _color3};

    u_int32_t third = _ledWrapper->getNumberLeds() / 3;
    u_int32_t counter = 0;
    u_int32_t  currentDivision = third;
    u_int8_t colorNumber = 1;

    for(u_int32_t count = 0; count < 3; count++)
    {
        ws2811_led_t currentColor = _color1;

        if (  (renderResults = _ledWrapper->clearLeds() ) == WS2811_SUCCESS)
        {
            for(counter = 0; counter < _ledWrapper->getNumberLeds(); counter++)
            {
                if(_running == false)
                    return;

                if(counter >= currentDivision)
                {
                    ++colorNumber;
                    currentDivision = third * colorNumber;
                    currentColor = colors[colorNumber];
                }

                _ledWrapper->setPixelColor(_settings->getStripHeight(), counter, currentColor);
                if( (renderResults = _ledWrapper->show()) != WS2811_SUCCESS)
                {
                    _logger->logWarning("LightSystem::ShowColor3 Render Failed");
                    return;
                }

                Ws2811Wrapper::waitMillSec(_wait);

            }

        }
    }
    _ledWrapper->clearLeds();
}


