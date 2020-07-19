#include "showcolor4.h"

ShowColor4::ShowColor4(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}


void ShowColor4::startShow()
{

    ws2811_led_t colors[] = {_color1, _color2, _color3, _color4};

    u_int32_t forths = _ledWrapper->getNumberLeds() / 4;
    u_int32_t  currentDivision = forths;
    u_int8_t colorNumber = 1;

    ws2811_led_t currentColor = _color1;

    for(u_int32_t counter = 0; counter < _ledWrapper->getNumberLeds(); counter++)
    {
        if(false == _running)
            return;

        if(counter >= currentDivision)
        {
            ++colorNumber;
            colorNumber = (colorNumber > 4) ? 4 : colorNumber;

            currentDivision = forths * colorNumber;
            currentColor = colors[colorNumber - 1];
        }

        _ledWrapper->setPixelColor(_settings->getStripHeight(), counter, currentColor);

    }
    _ledWrapper->show();

}


