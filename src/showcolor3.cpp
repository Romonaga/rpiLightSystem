#include "showcolor3.h"

ShowColor3::ShowColor3(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}


void ShowColor3::startShow()
{

    ws2811_led_t colors[] = {_color1, _color2, _color3};

    u_int32_t third = _ledWrapper->getNumberLeds() / 3;

    u_int32_t  currentDivision = third;
    u_int8_t colorNumber = 1;
    ws2811_led_t currentColor = _color1;

    for(u_int32_t counter = 0; counter < _ledWrapper->getNumberLeds(); counter++)
    {

        if(counter >= currentDivision)
        {
            ++colorNumber;
            colorNumber = (colorNumber > 3) ? 3 : colorNumber;
            currentDivision = third * colorNumber;
            currentColor = colors[colorNumber-1];
        }

        _ledWrapper->setPixelColor(_settings->getStripHeight(), counter, currentColor);

    }

    _ledWrapper->show();

}


