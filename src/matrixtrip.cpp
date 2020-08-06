#include "matrixtrip.h"
#include <QVector>
#include <math.h>
#include <QDebug>

#include "font7x6ext.h"


MatrixTrip::MatrixTrip(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}


void MatrixTrip::startShow()
{
    int rowMid =_settings->getChannels()[_channelId]->stripRows() / 2;
    int colMid = _settings->getChannels()[_channelId]->stripColumns() / 2;
    ws2811_led_t lastColor = 0;

    while(_endTime > time(nullptr))
    {

        for(int counter = (MAXROWS + 1); counter > 0; counter--)
        {
            do
            {
                _color1 = _ledWrapper->Wheel(( genRand(0, 256) + genRand(1, _ledWrapper->getNumberLeds())) & 255);

            }while(_color1 == lastColor);

            lastColor = _color1;
            circleBres(rowMid, colMid, counter);
            _ledWrapper->show();
            Ws2811Wrapper::waitMillSec(_wait);
        }


        if(_running == false || _endTime < time(nullptr))
            return;

    }
}

