#include "showcolor3.h"

ShowColor3::ShowColor3(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}


void ShowColor3::startShow()
{

    ws2811_return_t renderResults = WS2811_SUCCESS;
    ws2811_led_t colors[] = {_color1, _color2, _color3};

    u_int32_t third = _ledWrapper->getNumberLeds() / 3;


    for(u_int32_t count = 0; count < _numLoops; count++)
    {
        u_int32_t counter = 0;
        u_int32_t  currentDivision = third;
        u_int8_t colorNumber = 1;
        ws2811_led_t currentColor = _color1;
        _ledWrapper->clearLeds();
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
    
}


