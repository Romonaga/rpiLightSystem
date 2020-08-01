#include "showcolorevery.h"

ShowColorEvery::ShowColorEvery(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}


void ShowColorEvery::startShow()
{

    ws2811_return_t renderResults = WS2811_SUCCESS;

    for(u_int32_t counter = 0; counter < _ledWrapper->getNumberLeds(); counter++)
    {
            if(counter % _colorEvery == 0)
                _ledWrapper->setPixelColor(counter, _color1);

    }

    if( (renderResults = _ledWrapper->show()) != WS2811_SUCCESS)
    {
        _logger->logWarning("ShowColorEvery - Render Failed");
    }
  
}


