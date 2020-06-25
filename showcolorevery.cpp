#include "showcolorevery.h"

ShowColorEvery::ShowColorEvery(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}


void ShowColorEvery::startShow()
{

    ws2811_return_t renderResults = WS2811_SUCCESS;

    for(uint32_t i=0; i < _ledWrapper->getNumberLeds(); i++)
    {
        if(i % _colorEvery == 0)
        {
            _ledWrapper->setPixelColor(_settings->getStripHeight(), i, _color1);
        }

    }
        
    if( (renderResults = _ledWrapper->show()) != WS2811_SUCCESS)
    {
        _logger->logWarning("ShowColorEvery - Render Failed");
    }
  
}


