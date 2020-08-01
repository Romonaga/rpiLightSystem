#include "matrixscrolltext.h"
#include <QVector>
#include <math.h>
#include <QDebug>

#include "font7x6ext.h"



MatrixScrollText::MatrixScrollText(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}

void MatrixScrollText::shiftColumns(int rowStart, ws2811_led_t *image)
{
    ws2811_led_t color;

    for(int col = 1; col < _settings->getStripColumns() ; col++)
    {

        for(int row = rowStart; row < (MAXROWS + rowStart); row++)
        {
            color = _ledWrapper->getPixelColor(row, col);
            _ledWrapper->setPixelColor(row , col - 1, color);
         //   _ledWrapper->setPixelColor(row, col, image[(row - rowStart )* col]) ;
        }
    }

    _ledWrapper->show();
}



void MatrixScrollText::startShow()
{
    int bitCounter = 0;
    int columnStart = MAXCOLS;
    int drawCol = _settings->getStripColumns() - 1;
    int rowStart =  (_settings->getStripRows() / 2) - (MAXROWS / 2) - 1;
    int drawRow = 0;

    ws2811_led_t* image = new ws2811_led_t[MAXROWS * _settings->getStripColumns()];

    for(int col = 1; col < _settings->getStripColumns(); col++)
    {
        for(int row = 0; row < MAXROWS; row++)
        {
            image[row*col] = _ledWrapper->getPixelColor(row, col);
        }
    }

    while(_endTime > time(nullptr) && _running == true)
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
                    drawRow = row + rowStart;
                    bitCounter = row * MAXCOLS + (col - columnStart);

                    if(letterMatrix[(int)_matrixText.toStdString().c_str()[letter] - 32][bitCounter] == 1)
                    {
                        _ledWrapper->setPixelColor(drawRow, drawCol , _color1);
                    }
                    else
                       _ledWrapper->setPixelColor(drawRow, drawCol, image[row * (col - columnStart)]);
                       //_ledWrapper->setPixelColor(drawRow, drawCol, _ledWrapper->Color(0,0,0));
                }


                _ledWrapper->show();
                 Ws2811Wrapper::waitMillSec(_wait);
                 shiftColumns(rowStart, image);

            }

        }


        if(_running == false)
          break;

    }


    delete [] image;

}


