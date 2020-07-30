#include "matrixscrolltext.h"
#include <QVector>
#include <math.h>
#include <QDebug>

#include "font5x7ext.h"



MatrixScrollText::MatrixScrollText(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}

void MatrixScrollText::startShow()
{
    int bitCounter = 0;
    int row = 0;
    int startCol = _settings->getStripColumns() - MAXCOLS;


      for(int letter = 0; letter < _matrixText.length(); letter++)
      {

        if((int)_matrixText.toStdString().c_str()[letter] < 32 || (int)_matrixText.toStdString().c_str()[letter] > 122)
            continue;

        bitCounter = 0;
        _ledWrapper->clearLeds();

        for(int col = startCol; col < MAXCOLS + startCol; col++)
        {


            for(row = 0; row < MAXROWS; row++)
            {
                bitCounter = row * MAXCOLS + col - startCol;
                if(bitsPerLetter[(int)_matrixText.toStdString().c_str()[letter] - 32][bitCounter] == 1)
                    _ledWrapper->setPixelColor(row, col, _color1);
                else
                    _ledWrapper->setPixelColor(row, col, _ledWrapper->Color(0,0,0));
                //qDebug() << bitCounter;
            }

            _ledWrapper->show();
            Ws2811Wrapper::waitMillSec(_wait);


        }

    }


}


/*
void MatrixScrollText::startShow()
{
    int bitCounter = 0;
    int row = 0;


      for(int letter = 0; letter < _matrixText.length(); letter++)
      {

        if((int)_matrixText.toStdString().c_str()[letter] < 32 || (int)_matrixText.toStdString().c_str()[letter] > 122)
            continue;

        bitCounter = 0;
        _ledWrapper->clearLeds();

        for(int col = 0; col < MAXCOLS; col++)
        {


            for(row = 0; row < MAXROWS; row++)
            {
                bitCounter = row * MAXCOLS + col;
                if(bitsPerLetter[(int)_matrixText.toStdString().c_str()[letter] - 32][bitCounter] == 1)
                    _ledWrapper->setPixelColor(row, col, _color1);
                else
                    _ledWrapper->setPixelColor(row, col, _ledWrapper->Color(0,0,0));
                //qDebug() << bitCounter;
            }

            _ledWrapper->show();
            Ws2811Wrapper::waitMillSec(_wait);


        }

    }

}*/
