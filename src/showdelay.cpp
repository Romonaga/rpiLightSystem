#include "showdelay.h"

ShowDelay::ShowDelay(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}



void ShowDelay::startShow()
{
   Ws2811Wrapper::waitMillSec(_wait);
}



