#include "matrixsquare.h"
#include <QVector>
#include <math.h>
#include <QDebug>



MatrixSquare::MatrixSquare(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}


void MatrixSquare::startShow()
{
    QJsonDocument doc = QJsonDocument::fromJson(_showParms.toUtf8());

    int startRow = doc["startRow"].toString().toInt() - 1;
    int startCol = doc["startColumn"].toString().toInt() - 1;

    int length = doc["len"].toString().toInt();
    int height = doc["height"].toString().toInt();
    int fill = doc["fill"].toInt();
    int maxHeight = 0;



    if(fill == 0)
    {
        drawBox(startRow, startCol, length, height - 1);
    }
    else
    {
        maxHeight = height;

        for(int box = 1; box < maxHeight; box++)
        {

            qDebug() << "boxnum: " << box << " row: " << startRow << " col: " << startCol << "h: " << height << " len: " << length;
            drawBox(startRow++, startCol++, length--, --height);



        }
    }
    _ledWrapper->show();
}

