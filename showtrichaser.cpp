#include "showtrichaser.h"

ShowTriChaser::ShowTriChaser(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}


void ShowTriChaser::startShow()
{

    ws2811_return_t renderResults = WS2811_SUCCESS;
    ws2811_led_t curcolor = _color1;

    while(_endTime > time(nullptr))
    {

        for(uint32_t i=0; i < _ledWrapper->getNumberLeds(); i++)
        {
            if(i % 3 == 0)
                curcolor = _color3;
            else if(i % 2 == 0)
                curcolor = _color2;
            else
                curcolor = _color1;

            _ledWrapper->setPixelColor(_settings->getStripHeight(), i, curcolor);

            if( (renderResults = _ledWrapper->show()) != WS2811_SUCCESS)
            {
                _logger->logWarning("ShowTriChaser - Render Failed");
                break;

            }

            if(_running == false)
                return;

            Ws2811Wrapper::waitMillSec(_wait);

        }
        
        _ledWrapper->clearLeds();
    }
  
}


