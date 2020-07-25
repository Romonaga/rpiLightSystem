#include "showtheaterchaser.h"

ShowTheaterChaser::ShowTheaterChaser(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}


void ShowTheaterChaser::startShow()
{

    unsigned int j = 0;

    while(_endTime > time(nullptr))
    {
        if(_running == false)
            return;

        // cycle all 256 colors in the wheel
        for (int q=0; q < 3; q++)
        {
            for (u_int32_t i=0; i < _ledWrapper->getNumberLeds(); i=i+3)
            {
               _ledWrapper->setPixelColor(i+q, Ws2811Wrapper::Wheel( (i+j) % 255));    //turn every third pixel on
            }
            _ledWrapper->show();

          Ws2811Wrapper::waitMillSec(_wait);
          for (u_int32_t i=0; i < _ledWrapper->getNumberLeds(); i=i+3)
          {
            _ledWrapper->setPixelColor(i+q, 0);        //turn every third pixel off
          }

          j++;
        }

    }


}

