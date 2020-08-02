#include "matrixart.h"
#include <QVector>
#include <math.h>
#include <QDebug>



MatrixArt::MatrixArt(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}

void MatrixArt::startShow()
{

    QJsonObject jsonObject;
    QJsonObject jsonPixels;
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

                 _ledWrapper->show();
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


