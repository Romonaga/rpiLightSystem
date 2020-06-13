#include "showfire.h"
#include <QDebug>

ShowFire::ShowFire(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(settings,ledWrapper, lightShow, showParms)
{
   
}



void ShowFire::startShow()
{

      for(int counter = 0; counter < 255; counter++)
      {
          if(_running == false)
              return;// int r = 255, g = 50, b = 35;

        for (u_int32_t i = 0; i < _ledWrapper->getNumberLeds(); i++)
        {
          int flicker = (rand() % (100 - 0 + 1)) + 0;
          int r1 = _r - flicker;
          int g1 = _g - flicker;
          int b1 = _b - flicker;
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


