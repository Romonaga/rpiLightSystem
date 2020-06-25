#include "showrainbow.h"
#include <QDebug>

ShowRainbow::ShowRainbow(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}


void ShowRainbow::startShow()
{
    while(_endTime > time(nullptr))
    {
        _logger->logInfo("1");
        for(uint16_t wheel = 0; wheel < 256; wheel ++)
        {
            for(uint16_t ledCount = 0; ledCount <_ledWrapper->getNumberLeds(); ledCount++)
                _ledWrapper->setPixelColor(_settings->getStripHeight(), ledCount,  Ws2811Wrapper::Wheel((ledCount + wheel) & 255));

            _ledWrapper->show();
            qDebug() << "end: " << _endTime << " Now:: " << time(nullptr);
            if(_running == false || _endTime < time(nullptr))
                return;
            
            Ws2811Wrapper::waitMillSec(_wait);

        }
        Ws2811Wrapper::waitMillSec(_wait);
    }
}
/*
void ShowRainbow::startShow()
{

    u_int32_t i, j;


    for(int j=0; j < _numLoops; j++)
    {
      if(_running == false)
          return;

      for(i=0; i <_ledWrapper->getNumberLeds(); i++)
      {
        _ledWrapper->setPixelColor(_settings->getStripHeight(), i,  Ws2811Wrapper::Wheel((i+j) & 255));
      }
      _ledWrapper->show();
      if(_running == false)
          return;
      Ws2811Wrapper::waitMillSec(_wait);



    }


}

*/
