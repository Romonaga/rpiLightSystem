#include "matrixcircle.h"
#include <QVector>
#include <math.h>
#include <QDebug>



MatrixCircle::MatrixCircle(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}



void MatrixCircle::startShow()
{
    QJsonDocument doc = QJsonDocument::fromJson(_showParms.toUtf8());

    int startRow = doc["startRow"].toString().toInt();
    int startCol = doc["startColumn"].toString().toInt();
    int circleSize = doc["radius"].toString().toInt();
    int fill = doc["fill"].toInt();


    if(fill == 0)
    {
        //circleBres(startRow, startCol, circleSize);
        //dda_circle(startRow, startCol, circleSize);
        //circleMidpoint(startRow, startCol, circleSize);
        drawCircle(startRow, startCol, circleSize);
    }
    else
    {
        for(int counter = circleSize ; counter > 0; counter--)
        {
           // circleBres(startRow, startCol, counter);
            //dda_circle(startRow, startCol, counter);
          //  circleMidpoint(startRow, startCol, counter);

            drawCircle(startRow, startCol, counter);

        }

    }
    _ledWrapper->show();
}


