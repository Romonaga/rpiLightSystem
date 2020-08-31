#include "showscanner.h"

ShowScanner::ShowScanner(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{
   
}



ws2811_led_t ShowScanner::dimcolor(ws2811_led_t color, int width)
{
   return (((color&0xFF0000)/width)&0xFF0000) + (((color&0x00FF00)/width)&0x00FF00) + (((color&0x0000FF)/width)&0x0000FF);
}

void ShowScanner::startShow()
{

    ws2811_led_t old_val[_ledWrapper->getNumberLeds() ];

    while(_endTime > time(nullptr))
    {

          for (unsigned int count = 1; count < _ledWrapper->getNumberLeds(); count++)
          {
            _ledWrapper->setPixelColor(count, _color1);
            old_val[count] = _color1;

            for(int x = count; x>0; x--)
            {
              old_val[x-1] = dimcolor(old_val[x-1], _width);
             _ledWrapper->setPixelColor(x-1, old_val[x-1]);
            }
            _ledWrapper->show();

            Ws2811Wrapper::waitMillSec(_wait);
          }

          for (unsigned int count = _ledWrapper->getNumberLeds()-1; count == 0; count--)
          {
            _ledWrapper->setPixelColor(count, _color1);
            old_val[count] = _color1;

            for(unsigned int x = count; x<=_ledWrapper->getNumberLeds() ;x++)
            {
              old_val[x-1] = dimcolor(old_val[x-1], _width);
              _ledWrapper->setPixelColor(x+1, old_val[x+1]);
            }
            _ledWrapper->show();

            Ws2811Wrapper::waitMillSec(_wait);
          }
        }

}

