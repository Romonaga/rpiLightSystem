#include "matrixtrip.h"
#include <QVector>
#include <math.h>
#include <QDebug>

#include "pixalfonts.h"


MatrixTrip::MatrixTrip(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}


void MatrixTrip::startShow()
{
    if(_settings->getChannels()[_channelId]->matrixdirection() == 0) return;

    int rowMid =_settings->getChannels()[_channelId]->stripRows() / 2;
    int colMid = _settings->getChannels()[_channelId]->stripColumns() / 2;
    ws2811_led_t lastcolor = 0;

    while(_endTime > time(nullptr))
    {

        for(int counter = (rowMid + 1); counter > 0; counter--)
        {
            do
            {
                _color1 = _ledWrapper->wheel(( genRand(0, 256) + genRand(1, _ledWrapper->getNumberLeds())) & 255);

            }while(_color1 == lastcolor);

            lastcolor = _color1;
            drawCircle(rowMid, colMid, counter);
            _ledWrapper->show();
            Ws2811Wrapper::waitMillSec(_wait);
        }


        if(_running == false || _endTime < time(nullptr))
            return;

    }
}

