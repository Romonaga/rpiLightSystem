#include "showflame.h"

ShowFlame::ShowFlame(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{
   
}



void ShowFlame::startShow()
{

    while(_endTime > time(nullptr))
    {
        if(_running == false)
          return;

        for(u_int32_t counter = 0; counter < _ledWrapper->getNumberLeds(); counter++)
        {

              int flicker = (rand() % (100 - 0 + 1)) + 0;
              int r1 = _r - flicker;
              int g1 = _g - flicker;
              int b1 = _b - flicker;
              if (g1 < 0) g1 = 0;
              if (r1 < 0) r1 = 0;
              if (b1 < 0) b1 = 0;
              _ledWrapper->setPixelColor(counter, _ledWrapper->Color(r1, g1, b1));




            _ledWrapper->show();
            if(_running == false)
              return;
            //  Adjust the delay here, if you'd like.  Right now, it randomizes the
            //  color switch delay to give a sense of realism
            Ws2811Wrapper::waitMillSec((rand() % (150 - 20 + 1)) + 20);

        }
    }

}

