#include "showbouncingballs.h"
#include <QDebug>


#include <QDateTime>
#include <QTime>


#include "pixalfonts.h"


using namespace std;



ShowBouncingBalls::ShowBouncingBalls(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}

void ShowBouncingBalls::startShow()
{


}
/*

void ShowBouncingBalls::startShow()
{
    uint32_t snapshotBufferSize = 0;
    int  drawCol  = 0;
    _rowStart = 0;
    ws2811_led_t* snapShotBuffer = nullptr;



    QTime qtime;
    QString timeStr;

    snapShotBuffer = snapShot(_rowStart, CLOCKMAXROWS, &snapshotBufferSize);
    if(snapShotBuffer != nullptr)
    {
        drawCol = _settings->getChannels()[_channelId]->stripColumns() - 1;


        while(_endTime > time(nullptr) && _running == true)
        {
            timeStr = qtime.currentTime().toString().toStdString().c_str();

            for(int letter = 0; letter < timeStr.length(); letter++)
            {

                qDebug() << "val: " << (int)timeStr.toStdString().c_str()[letter] << "let: " << (int)/*timeStr.toStdString().c_str()[letter]
                if((int)timeStr.toStdString().c_str()[letter] < 48 || (int)timeStr.toStdString().c_str()[letter] > 59) //only attempt to print what we know.
                    continue;

                for(int col = CLOCKMAXCOLS; col < (CLOCKMAXCOLS * 2); col++)
                {
                    for(int row = 0; row < CLOCKMAXROWS; row++)
                    {


                        if(clockMatrix[(int)timeStr.toStdString().c_str()[letter] - 48][row * CLOCKMAXCOLS + (col - CLOCKMAXCOLS)] == 1) //should this pixal be on?
                        {
                            _ledWrapper->setPixelColor(row + _rowStart, drawCol , _color1);
                        }
                    }


                    _ledWrapper->show();

                    Ws2811Wrapper::waitMillSec(_wait);

                    shiftColumns(CLOCKMAXROWS, _rowStart, _color1, snapShotBuffer);

                }
            }

        }

        delete [] snapShotBuffer;
    }


}


*/
