#include "showchaser.h"

ShowChaser::ShowChaser(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(settings,ledWrapper, lightShow, showParms)
{
    _color = _ledWrapper->Color(125, 12 , 170);
    _wait = 20;

}



void ShowChaser::startShow()
{

    ws2811_return_t renderResults;

    _ledWrapper->clearLeds();
    for(int count = 0; count < 10; count++)
    {
        for(u_int32_t counter = 0; counter < _ledWrapper->getNumberLeds(); counter++)
        {
            _ledWrapper->setPixelColor(_settings->getStripHeight(), counter, _color);
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
         _ledWrapper->clearLeds();
    }
    _ledWrapper->clearLeds();
}


