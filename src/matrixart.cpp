#include "matrixart.h"
#include <QVector>
#include <math.h>
#include <QDebug>
#include <sstream>


MatrixArt::MatrixArt(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}

void MatrixArt::startShow()
{

    QJsonObject jsonObject;
    QJsonObject jsonPixels;
    int index = 0;
    bool resizeNeeded  = false;

    ws2811_led_t* snapShotBuffer = nullptr;
    ws2811_led_t* reSampledImageData = nullptr;

    uint32_t snapShotbufferSize = 0;
    int shiftCols = 0;
    bool moveLeft = false;


    QJsonDocument doc = QJsonDocument::fromJson(_showParms.toUtf8());
    if(!doc.isNull() && doc.isObject())
    {
         jsonObject = doc.object();

         //lets paint
         if(jsonObject["pixles"].isObject() == true)
         {

             jsonPixels = jsonObject["pixles"].toObject();
             shiftCols = jsonObject["sc"].toInt();

             moveLeft = (shiftCols >= 0) ? true : false;
             foreach(const QJsonValue &value, jsonPixels)
             {
                 try
                 {
                      if(moveLeft == true && value["c"].toInt() < shiftCols) continue; //some ability to correct art, will add other shifts soon.
                      if(moveLeft == false && (value["c"].toInt() + -shiftCols ) > (int)(_ledWrapper->getColumns() - 1 )) continue;

                      _ledWrapper->setPixelColor(value["r"].toInt(), value["c"].toInt() + -shiftCols, std::stoul(value["co"].toString().replace("#","0x").toStdString().c_str(), nullptr, 16));
                 }
                 catch (const std::invalid_argument&)
                 {
                     std::stringstream info;
                     info << "MatrixArt could not decode value, stoppping. row: " << value["r"].toInt() << " col: " << value["c"].toInt() + -shiftCols;
                     _logger->logInfo(info.str());
                     break;
                 }
             }

            //build old image from json
            uint32_t columns = (jsonObject["stripColumns"].isString()) ? jsonObject["stripColumns"].toString().toInt() : _ledWrapper->getColumns();
            uint32_t rows = (int)(jsonPixels.length() / columns);
            snapShotBuffer = snapShot(0, rows, columns, &snapShotbufferSize);

            if(snapShotBuffer != nullptr)
            {
                //will it fit, or do we make it ugly?  Always shrink, never expand.
                resizeNeeded =  (columns > _ledWrapper->getColumns() || rows > _ledWrapper->getRows());
                if(resizeNeeded == true)
                {
                    _logger->logInfo("** MatrixArt resize required.");
                    reSampledImageData = resampleColor(_ledWrapper->getColumns(), _ledWrapper->getRows(), columns, rows, snapShotBuffer);

                    columns     = _ledWrapper->getColumns();
                    rows        =  _ledWrapper->getRows();

                    delete [] snapShotBuffer;
                    if(reSampledImageData != nullptr)
                        snapShotBuffer = reSampledImageData;
                }

                //Well now that we are done playing, lets display it.
                if(snapShotBuffer != nullptr)
                {

                     for(uint32_t row = 0; row < rows; row++)
                     {
                         for(uint32_t col = 0 ; col < columns; col++)
                         {
                             _ledWrapper->setPixelColor(row, col ,  snapShotBuffer[index]);
                             index++;
                         }
                     }

                     delete [] snapShotBuffer;
                     _ledWrapper->show();

                }
                else
                {
                    _logger->logError("MatrixArt No Resample Buffer!");
                }

            }
            else
            {
                _logger->logError("MatrixArt No SnapshotBuffer!");
            }

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

