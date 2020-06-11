#include "showfire.h"

ShowFire::ShowFire(SystemSettings* settings, Ws2811Wrapper* ledWrapper, QObject *parent) : ILightShow(settings,ledWrapper, parent)
{

}

bool ShowFire::showParms(const QString &showParms)
{
    return true;
}

void ShowFire::startShow()
{

      //int r = 255, g = 55, b = 0;
      //  Purple flame:
      int r = 165, g = 8, b = 148;

      //  Green flame:
      //int r = 74, g = 150, b = 12;
      //  Flicker, based on our initial RGB values
      for(int counter = 0; counter < 255; counter++)
      {
          if(_running == false)
              return;// int r = 255, g = 50, b = 35;

        for (u_int32_t i = 0; i < _ledWrapper->getNumberLeds(); i++)
        {
          int flicker = (rand() % (100 - 0 + 1)) + 0;
          int r1 = r - flicker;
          int g1 = g - flicker;
          int b1 = b - flicker;
          if (g1 < 0) g1 = 0;
          if (r1 < 0) r1 = 0;
          if (b1 < 0) b1 = 0;
          _ledWrapper->setPixelColor(_settings->getStripHeight(), i, r1, g1, b1);
        }
        _ledWrapper->show();

        if(_running == false)
            return;
        //  Adjust the delay here, if you'd like.  Right now, it randomizes the
        //  color switch delay to give a sense of realism
        Ws2811Wrapper::waitMillSec((rand() % (150 - 20 + 1)) + 20);
      }
      _ledWrapper->clearLeds();
}

void ShowFire::stopShow()
{
    _running = false;
}

