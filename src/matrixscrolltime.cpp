#include "matrixscrolltime.h"
#include <QDebug>


#include <QDateTime>
#include <QTime>


#include "pixalfonts.h"


using namespace std;



MatrixScrollTime::MatrixScrollTime(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

    _rowStart =  (_settings->getChannels()[_channelId]->stripRows()/ 2) - (CLOCKMAXROWS / 2) - 1;
    _drawCol = _settings->getChannels()[_channelId]->stripColumns() - 1;

}

void MatrixScrollTime::scrollTime(QString msg, int maxRows, int maxCols, int rowStart, int drawCol, ws2811_led_t color,  ws2811_led_t *snapShotBuffer, int delay)
{

    if(snapShotBuffer == nullptr) return;

    for(int letter = 0; letter < msg.length(); letter++)
    {

        if((int)msg.toStdString().c_str()[letter] < 32 || (int)msg.toStdString().c_str()[letter] > 59) //only attempt to print what we know.
            continue;


        for(int col = maxCols; col < (maxCols * 2); col++)
        {
            for(int row = 0; row < maxRows; row++)
            {

                if(timeMatrix[(int)msg.toStdString().c_str()[letter] - 32][row * maxCols + (col - maxCols)] == 1) //should this pixal be on?
                    _ledWrapper->setPixelColor(row + rowStart, drawCol , color);
            }


            _ledWrapper->show();

            Ws2811Wrapper::waitMillSec(delay);

            shiftColumns(maxRows, rowStart, color, snapShotBuffer);

        }
    }

}




void MatrixScrollTime::startShow()
{

    if(_settings->getChannels()[_channelId]->matrixdirection() == 0) return;


    QString sendPad;
    uint32_t snapshotBufferSize;
    ws2811_led_t* snapShotBuffer = nullptr;

    QTime qtime;
    QString timeStr;


    for(int pad = 0; pad < _settings->getChannels()[_channelId]->stripColumns() / CLOCKMAXCOLS; pad++)       //pad the string so it will scroll off the screen
        sendPad.append(" ");

    if(_showParmsJson.value("position").isString())
    {

        switch(_showParmsJson.value("position").toString().toInt())
        {

        case 1:
           _rowStart = 0;
        break;

        case 2:
           _rowStart =  (_settings->getChannels()[_channelId]->stripRows() / 2) - (CLOCKMAXROWS / 2) - 1;
        break;

        case 3:
           _rowStart = _settings->getChannels()[_channelId]->stripRows() - CLOCKMAXROWS;
        break;


        }
    }


    snapShotBuffer = snapShot(_rowStart, CLOCKMAXROWS, _settings->getChannels()[_channelId]->stripColumns(), &snapshotBufferSize);
    if(snapShotBuffer != nullptr)
    {
        while(_endTime > time(nullptr) && _running == true)
        {
            timeStr = qtime.currentTime().toString().toStdString().c_str();
            timeStr.append(sendPad);

            scrollTime(timeStr, CLOCKMAXROWS, CLOCKMAXCOLS, _rowStart, _drawCol, _color1, snapShotBuffer, _wait);
        }

        replaySnapShot(_rowStart, CLOCKMAXROWS, _settings->getChannels()[_channelId]->stripColumns(), snapShotBuffer);
        delete [] snapShotBuffer;
    }

}




/*

void MatrixScrollTime::startShow()
{
    uint32_t snapshotBufferSize = 0;
    int  drawCol  = 0;
    _rowStart = 0;
    ws2811_led_t* snapShotBuffer = nullptr;
    QString sendPad;

    QTime qtime;
    QString timeStr;

    for(int pad = 0; pad < _settings->getChannels()[_channelId]->stripColumns() / CLOCKMAXCOLS; pad++)       //pad the string so it will scroll off the screen
       sendPad.append(" ");

    if(_showParmsJson.value("position").isString())
    {

        switch(_showParmsJson.value("position").toString().toInt())
        {

        case 1:
           _rowStart = 0;
        break;

        case 2:
           _rowStart =  (_settings->getChannels()[_channelId]->stripRows() / 2) - (CLOCKMAXROWS / 2) - 1;
        break;

        case 3:
           _rowStart = _settings->getChannels()[_channelId]->stripRows() - CLOCKMAXROWS;
        break;


        }
    }

    snapShotBuffer = snapShot(_rowStart, CLOCKMAXROWS, CLOCKMAXCOLS, &snapshotBufferSize);
    if(snapShotBuffer != nullptr)
    {
        drawCol = _settings->getChannels()[_channelId]->stripColumns() - 1;


        while(_endTime > time(nullptr) && _running == true)
        {
            timeStr = qtime.currentTime().toString().toStdString().c_str();
            timeStr.append(sendPad);

            for(int letter = 0; letter < timeStr.length(); letter++)
            {

                if((int)timeStr.toStdString().c_str()[letter] < 32 || (int)timeStr.toStdString().c_str()[letter] > 59) //only attempt to print what we know.
                    continue;

                for(int col = CLOCKMAXCOLS; col < (CLOCKMAXCOLS * 2); col++)
                {
                    for(int row = 0; row < CLOCKMAXROWS; row++)
                    {

                        if(clockMatrix[(int)timeStr.toStdString().c_str()[letter] - 32][row * CLOCKMAXCOLS + (col - CLOCKMAXCOLS)] == 1) //should this pixal be on?
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
