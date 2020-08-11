#include "matrixtriangle.h"
#include <QVector>
#include <math.h>
#include <QDebug>



MatrixTriangle::MatrixTriangle(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}


void MatrixTriangle::startShow()
{
    QJsonDocument doc = QJsonDocument::fromJson(_showParms.toUtf8());

    int startRow = doc["startRow"].toString().toInt();
    int startCol = doc["startColumn"].toString().toInt();

    int length = doc["len"].toString().toInt();

    int fill = doc["fill"].toInt();
    int direction = 1;

     if(_showParmsJson.value("direction").isString())
         direction = _showParmsJson.value("direction").toString().toInt();



    if(fill == 0)
    {
        drawTriangle(startRow, startCol, length , direction);
    }
    else
    {
        for(int counter = length ; counter > 1; counter--)
        {
            drawTriangle(startRow, startCol, counter , direction);
        }

    }
    _ledWrapper->show();
}

