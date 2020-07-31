#include "matrixscrolltext.h"
#include <QVector>
#include <math.h>
#include <QDebug>

#include "font5x7ext.h"



MatrixScrollText::MatrixScrollText(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}

void MatrixScrollText::shiftColumns()
{
    ws2811_led_t color;
    ws2811_led_t blackColor = _ledWrapper->Color(0,0,0);

    for(int col = 1; col < _settings->getStripColumns() ; col++)
    {

        for(int row = 0; row < _settings->getStripRows(); row++)
        {
            color = _ledWrapper->getPixelColor(row, col);


            _ledWrapper->setPixelColor(row , col - 1, color);
         _ledWrapper->setPixelColor(row, col,blackColor );
        }
    }

    _ledWrapper->show();
}



void MatrixScrollText::startShow()
{
    int bitCounter = 0;
    int columnStart = 5;
    int drawCol = _settings->getStripColumns() - 1;
    _ledWrapper->clearLeds();

    while(_endTime > time(nullptr))
    {
        for(int letter = 0; letter < _matrixText.length(); letter++)
        {

            if((int)_matrixText.toStdString().c_str()[letter] < 32 || (int)_matrixText.toStdString().c_str()[letter] > 122)
                continue;

            bitCounter = 0;
            for(int col = columnStart; col < (MAXCOLS + columnStart); col++)
            {

                for(int row = 0; row < MAXROWS; row++)
                {

                    bitCounter = row * MAXCOLS + (col - columnStart);
                    if(bitsPerLetter[(int)_matrixText.toStdString().c_str()[letter] - 32][bitCounter] == 1)
                        _ledWrapper->setPixelColor(row, drawCol , _color1);
                    else
                        _ledWrapper->setPixelColor(row, drawCol, _ledWrapper->Color(0,0,0));
                }


                _ledWrapper->show();
                 Ws2811Wrapper::waitMillSec(_wait);
                 shiftColumns();

            }

        }

        if(_running == false)
          return;

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
