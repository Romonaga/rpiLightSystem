#include "showcolorevery.h"

ShowColorEvery::ShowColorEvery(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}


void ShowColorEvery::startShow()
{

    ws2811_return_t renderResults = WS2811_SUCCESS;

    for(u_int32_t row = 0; row <= _settings->getStripRows(); row++)
    {
        for(u_int32_t column = 0; column <= _settings->getStripColumns(); column++)
        {
            if(column % _colorEvery == 0)
            {
                _ledWrapper->setPixelColor(row, column, _color1);
            }
        }
    }

/*    for(uint32_t i=0; i < _ledWrapper->getNumberLeds(); i++)
    {
        if(i % _colorEvery == 0)
        {
            _ledWrapper->setPixelColor(_settings->getStripHeight(), i, _color1);
        }

    }
  */

    if( (renderResults = _ledWrapper->show()) != WS2811_SUCCESS)
    {
        _logger->logWarning("ShowColorEvery - Render Failed");
    }
  
}


