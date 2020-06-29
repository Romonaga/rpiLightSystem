#include "showchaser.h"

ShowChaser::ShowChaser(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{
   
}



void ShowChaser::startShow()
{

    ws2811_return_t renderResults;

    while(_endTime > time(nullptr))
    {
        
        for(u_int32_t row = 1; row <= _ledWrapper->getHight(); row++)
        {
           for (u_int32_t led = 0; led < _ledWrapper->getWidth(); led++)
           {
               _ledWrapper->setPixelColor(row, led, _color1);
               renderResults = _ledWrapper->show();
               if(renderResults != WS2811_SUCCESS)
               {
                   _logger->logWarning(_ledWrapper->ws2811_get_return_t_str(renderResults));
                   return;
               }
               if(_running == false)
                   return;
               Ws2811Wrapper::waitMillSec(_wait);

            }
        }
        _ledWrapper->clearLeds();
    }
}


