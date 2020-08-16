#include "matrixart.h"
#include <QVector>
#include <math.h>
#include <QDebug>



MatrixArt::MatrixArt(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}

/*
void MatrixArt::startShow()
{

    QJsonObject jsonObject;
    QJsonObject jsonPixels;
    int index = 0;
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;
    ws2811_led_t color = 0;


    QJsonDocument doc = QJsonDocument::fromJson(_showParms.toUtf8());

    if(!doc.isNull())
    {
        if(doc.isObject())
        {
             jsonObject = doc.object();

             if(jsonObject["pixles"].isObject())
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
                snapShot(0, rows);

                //resample it
                unsigned char* reSampledImageData = (unsigned char*)resampleNew(_settings->getChannels()[_channelId]->stripColumns() ,_settings->getChannels()[_channelId]->stripRows() , columns, rows, (unsigned char*)_image);
                if(reSampledImageData != nullptr)
                {
                     for(int row = 0; row < _settings->getChannels()[_channelId]->stripRows(); row++)
                     {
                         for(int col = 0; col < _settings->getChannels()[_channelId]->stripColumns(); col++)
                         {
                             //ws2811_led_t ledColor = _ledWrapper->Color(reSampledImageData[index], reSampledImageData[index + 1], reSampledImageData[index + 2]);
                             _ledWrapper->setPixelColor(row, col,  reSampledImageData[index]);
                             index++;
                         }
                     }

                     delete [] reSampledImageData;
                }

                 deleteSnapShot();
                 //delete [] imageData;
                 _ledWrapper->show();
             }
             else
             {

                 _logger->logInfo("MatrixArt We have no pixals!");
             }

        }
        else
        {
            _logger->logInfo("ShowMatrix Invalid JSON");
        }
    }
    else
    {
          _logger->logWarning("ShowMatrix Document is not an object");
    }

}

*/

void MatrixArt::startShow()
{

    QJsonObject jsonObject;
    QJsonObject jsonPixels;
    int index = 0;
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;
    ws2811_led_t color = 0;


    QJsonDocument doc = QJsonDocument::fromJson(_showParms.toUtf8());

    if(!doc.isNull())
    {
        if(doc.isObject())
        {
             jsonObject = doc.object();

             if(jsonObject["pixles"].isObject())
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
                snapShot(0, rows);
                unsigned char* imageData = new unsigned char[getSnapShotBufferSize() * 3];
                unsigned int bufferIndex = 0;
                for(int counter = 0; counter < jsonPixels.length(); counter++)
                {
                     color = _image[counter];

                     red =  _ledWrapper->Red(color);
                     imageData[bufferIndex++] = red;

                     green =  _ledWrapper->Green(color);
                     imageData[bufferIndex++] = green;

                     blue =  _ledWrapper->Blue(color);
                     imageData[bufferIndex++] = blue;
                }

                //resample it
                unsigned char* reSampledImageData = resample(_settings->getChannels()[_channelId]->stripColumns() ,_settings->getChannels()[_channelId]->stripRows() , columns, rows, imageData);
                if(reSampledImageData != nullptr)
                {
                     for(int row = 0; row < _settings->getChannels()[_channelId]->stripRows(); row++)
                     {
                         for(int col = 0; col < _settings->getChannels()[_channelId]->stripColumns(); col++)
                         {
                             ws2811_led_t ledColor = _ledWrapper->Color(reSampledImageData[index], reSampledImageData[index + 1], reSampledImageData[index + 2]);
                             _ledWrapper->setPixelColor(row, col,  ledColor);
                             index += 3;
                         }
                     }

                     delete [] reSampledImageData;
                }

                 deleteSnapShot();
                 delete [] imageData;
                 _ledWrapper->show();
             }
             else
             {

                 _logger->logInfo("MatrixArt We have no pixals!");
             }

        }
        else
        {
            _logger->logInfo("ShowMatrix Invalid JSON");
        }
    }
    else
    {
          _logger->logWarning("ShowMatrix Document is not an object");
    }

}

