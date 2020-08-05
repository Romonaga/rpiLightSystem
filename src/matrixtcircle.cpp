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

    int startRow = doc["startRow"].toInt();
    int startCol = doc["startCol"].toInt();
    int circleSize = doc["startCol"].toInt();
    int fill = doc["startCol"].toInt();

    if(fill == 0)
    {
        circleBres(startRow, startCol, circleSize);
    }
    else
    {
        for(int counter = (circleSize + 1); counter > 0; counter--)
        {
            circleBres(startRow, startCol, counter);

        }
        _ledWrapper->show();
    }

}

