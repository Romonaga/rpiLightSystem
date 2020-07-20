#include "showscanner.h"

ShowScanner::ShowScanner(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{
   
}

/*
void knightRider(uint16_t cycles, uint16_t speed, uint8_t width, uint32_t color) {
  uint32_t old_val[NUM_PIXELS]; // up to 256 lights!
  // Larson time baby!
  for(int i = 0; i < cycles; i++){
    for (int count = 1; count<NUM_PIXELS; count++) {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      for(int x = count; x>0; x--) {
        old_val[x-1] = dimColor(old_val[x-1], width);
        strip.setPixelColor(x-1, old_val[x-1]);
      }
      strip.show();
      delay(speed);
    }
    for (int count = NUM_PIXELS-1; count>=0; count--) {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      for(int x = count; x<=NUM_PIXELS ;x++) {
        old_val[x-1] = dimColor(old_val[x-1], width);
        strip.setPixelColor(x+1, old_val[x+1]);
      }
      strip.show();
      delay(speed);
    }
  }
}

void clearStrip() {
  for( int i = 0; i<NUM_PIXELS; i++){
    strip.setPixelColor(i, 0x000000); strip.show();
  }
}

uint32_t dimColor(uint32_t color, uint8_t width) {
   return (((color&0xFF0000)/width)&0xFF0000) + (((color&0x00FF00)/width)&0x00FF00) + (((color&0x0000FF)/width)&0x0000FF);
}
*/


ws2811_led_t ShowScanner::dimColor(ws2811_led_t color, int width)
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
            _ledWrapper->setPixelColor(_settings->getStripHeight(), count, _color1);
            old_val[count] = _color1;

            for(int x = count; x>0; x--)
            {
              old_val[x-1] = dimColor(old_val[x-1], _width);
             _ledWrapper->setPixelColor(_settings->getStripHeight(),x-1, old_val[x-1]);
            }
            _ledWrapper->show();

            Ws2811Wrapper::waitMillSec(_wait);
          }

          for (unsigned int count = _ledWrapper->getNumberLeds()-1; count == 0; count--)
          {
            _ledWrapper->setPixelColor(_settings->getStripHeight(),count, _color1);
            old_val[count] = _color1;

            for(unsigned int x = count; x<=_ledWrapper->getNumberLeds() ;x++)
            {
              old_val[x-1] = dimColor(old_val[x-1], _width);
              _ledWrapper->setPixelColor(_settings->getStripHeight(),x+1, old_val[x+1]);
            }
            _ledWrapper->show();

            Ws2811Wrapper::waitMillSec(_wait);
          }
        }

}

