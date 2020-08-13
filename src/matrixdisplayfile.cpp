#include "matrixdisplayfile.h"
#include <QVector>
#include <QSqlError>
#include <QFileInfo>
#include <QStringLiteral>
#include <math.h>
#include <QDebug>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include "lodepng.h"
#include "jpgd.h"



MatrixCreateDisplayFile::MatrixCreateDisplayFile(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}

void MatrixCreateDisplayFile::saveArt()
{

   /* QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL","saveArt");
    database.setHostName(_settings->getDBServer());
    database.setUserName(_settings->getDBUser());
    database.setPassword(_settings->getDBPwd());
    database.setDatabaseName(_settings->getDataBase());

    if(database.open())
    {
        std::stringstream sql;
        sql << "insert into matrixArt(userId, artName, showParms, enabled) values('1','" << _fileName.toStdString().c_str() << "','" << _jsondoc->toJson().toStdString().c_str() << "','1');";
        QSqlQuery result(sql.str().c_str(), database);
        if(result.lastError().type() != QSqlError::NoError)
        {
            _logger->logInfo(result.lastError().text().toStdString());
        }
        database.close();
    }
    else
    {
        _logger->logInfo(database.lastError().text().toStdString());
    }
    */
}


void MatrixCreateDisplayFile::startShow()
{

    unsigned char* imageData = nullptr;
    unsigned char* reSampledImageData = nullptr;

    int comps = 3;
    int index = 0;
    unsigned error = 1;
    int pixalCount = 0;
    QJsonDocument jsondoc =  QJsonDocument();
    QJsonObject jsonObj = jsondoc.object();
    QJsonArray pixals = QJsonArray();


    QString fileName = _showParmsJson.value("uploadArt").toString();
    int saveAsArt = _showParmsJson.value("saveArt").toInt();

    if(fileName.isEmpty() == false)
    {
        QFileInfo file(fileName);
        if(file.exists())
        {
            if((file.suffix().toUpper() == "PNG"))
            {
                unsigned int width = 0;
                unsigned int height = 0;

                error = lodepng_decode24_file(&imageData, &width, &height, fileName.toStdString().c_str());
                if(error == 0 && width > 0 && height > 0 && imageData != nullptr)
                {
                    reSampledImageData =  resample(_settings->getChannels()[_channelId]->stripColumns(),_settings->getChannels()[_channelId]->stripRows(), width, height, imageData);
                    free(imageData);
                }

            }
            else if(file.suffix().toUpper() == "JPG")
            {
                int width = 0;
                int height = 0;
                imageData = jpgd::decompress_jpeg_image_from_file(fileName.toStdString().c_str(), &width, &height, &comps, 3 );
                if(imageData != nullptr && width > 0 && height > 0 )
                {
                    error = 0;
                    reSampledImageData =  resample(_settings->getChannels()[_channelId]->stripColumns(),_settings->getChannels()[_channelId]->stripRows(), width, height, imageData);
                    delete [] imageData;
                }

            }


            QJsonObject pixal = jsondoc.object();

            if(error == 0 && reSampledImageData != nullptr)
            {
                for(int row = 0; row < _settings->getChannels()[_channelId]->stripRows(); row++)
                {

                    for(int col = 0; col < _settings->getChannels()[_channelId]->stripColumns(); col++)
                    {
                        ws2811_led_t ledColor = _ledWrapper->Color(reSampledImageData[index], reSampledImageData[index + 1], reSampledImageData[index + 2]);
                        if(saveAsArt == 1)
                        {
                            pixalCount++;
                            QJsonObject color = jsondoc.object();

                            color["r"] = row;
                            color["c"] = col;
                            QString number = QString("0x").append(QStringLiteral("%1").arg(ledColor, 8, 16, QLatin1Char('0')));

                          //  color["co"] = QString("0x").append(QString().number(ledColor, 16));
                            color["co"] = QString("0x").append(QStringLiteral("%1").arg(ledColor, 6, 16, QLatin1Char('0')));
                            pixal[QString().number(pixalCount)] = color;


                        }
                        _ledWrapper->setPixelColor(row, col,  ledColor);
                        index += 3;

                    }
                }

                if(saveAsArt == 1)
                {
                    jsonObj["show"] = QString().number((int)MatrixArtShow);
                    jsonObj["userID"] = QString().number(1);
                    jsonObj["pixles"] = pixal;
                    jsonObj["systemId"] = QString().number(_settings->getSystemId());
                    jsonObj["channelId"] = QString().number(_channelId + 1);
                    jsonObj["brightness"] = QString().number(_brightness);
                    jsondoc.setObject(jsonObj);

                    QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL","saveArt");
                    database.setHostName(_settings->getDBServer());
                    database.setUserName(_settings->getDBUser());
                    database.setPassword(_settings->getDBPwd());
                    database.setDatabaseName(_settings->getDataBase());

                    if(database.open())
                    {
                        std::stringstream sql;
                        sql << "insert into matrixArt(userId, artName, showParms, enabled) values('1','" << file.baseName().toStdString().c_str() << "','" << jsondoc.toJson().toStdString().c_str() << "','1');";
                        QSqlQuery result(sql.str().c_str(), database);
                        if(result.lastError().type() != QSqlError::NoError)
                        {
                            _logger->logInfo(result.lastError().text().toStdString());
                        }
                        database.close();
                    }
                    else
                    {
                        _logger->logInfo(database.lastError().text().toStdString());
                    }

                }

                _ledWrapper->show();

                delete [] reSampledImageData;

            }

        }
    }
}
