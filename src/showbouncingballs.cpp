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

    QJsonObject jsonObject;
    QJsonObject jsonPixels;
    int index = 0;
    ws2811_led_t* snapShotBuffer = nullptr;
    uint32_t snapShotbufferSize = 0;


    QJsonDocument doc = QJsonDocument::fromJson(_showParms.toUtf8());
    if(!doc.isNull() && doc.isObject())
    {
         jsonObject = doc.object();

         if(jsonObject["pixles"].isObject() == true)
         {

             jsonPixels = jsonObject["pixles"].toObject();
             foreach(const QJsonValue &value, jsonPixels)
             {
                 try
                 {
                    _ledWrapper->setPixelColor(value["r"].toInt(), value["c"].toInt(), std::stoul(value["co"].toString().replace("#","0x").toStdString().c_str(), nullptr, 16));
                 }
                 catch (const std::invalid_argument&)
                 {
                     _logger->logInfo("MatrixArt could not decode value, stoppping.");
                     return;
                 }
             }

            //build old image
            int columns = (jsonObject["stripColumns"].isString()) ? jsonObject["stripColumns"].toString().toInt() : _ledWrapper->getColumns();
            int rows = (int)(jsonPixels.length() / columns);

            snapShotBuffer = snapShot(0, rows, columns, &snapShotbufferSize);

             for(int row = 0; row < rows; row++)
             {
                 for(int col = 0; col < columns; col++)
                 {
                     _ledWrapper->setPixelColor(row, col,  snapShotBuffer[index]);
                     index++;
                 }
             }

            _ledWrapper->show();

            }
            else
             {
                 _logger->logInfo("MatrixArt We have no pixals!");
             }

    }
    else
    {
          _logger->logWarning("ShowMatrix Document is not Valid");
    }

}

/*
void ShowBouncingBalls::startShow()
{
    uint32_t snapshotBufferSize = 0;
    int  drawCol  = 0;
    _rowStart = 0;
    ws2811_led_t* snapShotBuffer = nullptr;
    QString sendPad;


    QTime qtime;
    QString timeStr;

    snapShotBuffer = snapShot(_rowStart, CLOCKMAXROWS, &snapshotBufferSize);
    if(snapShotBuffer != nullptr)
    {
        drawCol = _settings->getChannels()[_channelId]->stripColumns() - 1;


        while(_endTime > time(nullptr) && _running == true)
        {
            timeStr = qtime.currentTime().toString().toStdString().c_str();
            for(int pad = 0; pad < _settings->getChannels()[_channelId]->stripColumns() / CLOCKMAXCOLS; pad++)       //pad the string so it will scroll off the screen
                sendPad.append(" ");

            timeStr.append(sendPad);
            for(int letter = 0; letter < timeStr.length(); letter++)
            {

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
