#include "showbouncingballs.h"

#include <QDebug>

ShowBouncingBalls::ShowBouncingBalls(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}

ShowBouncingBalls::~ShowBouncingBalls()
{

}

void ShowBouncingBalls::startShow()
{


}
