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
    _logger->logInfo("ShowMatrix Started");

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

//                     qDebug() << "row: " << value["r"].toInt() << "col: " << value["c"].toInt() << "color: " << std::stoul(value["co"].toString().toStdString().c_str(), nullptr, 16);

                     _ledWrapper->setPixelColor(value["r"].toInt(), value["c"].toInt(), std::stoul(value["co"].toString().replace("#","0x").toStdString().c_str(), nullptr, 16));
                 }

                 _ledWrapper->show();
                 _logger->logInfo("we have pixles!");
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


