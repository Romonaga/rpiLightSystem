#include "matrixscrolltext.h"
#include <QVector>
#include <math.h>
#include <QDebug>

#include "font7x6ext.h"



MatrixScrollText::MatrixScrollText(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{
    _image = new ws2811_led_t[MAXROWS * _settings->getStripColumns()];
    _rowStart =  (_settings->getStripRows() / 2) - (MAXROWS / 2) - 1;
    _drawCol = _settings->getStripColumns() - 1;

    for(int col = 0; col < _settings->getStripColumns(); col++)
    {
        for(int row = _rowStart; row < (MAXROWS + _rowStart); row++)
        {
            _image[(row - _rowStart) *col] = _ledWrapper->getPixelColor(row, col);
        }
    }

}

MatrixScrollText::~MatrixScrollText()
{
    delete [] _image;
}

void MatrixScrollText::shiftColumns()
{
    ws2811_led_t color;
    ws2811_led_t off = _ledWrapper->Color(0,0,0);
    int drawRow = 0;
    int past = 0;

    for(int current = 0; current  < _settings->getStripColumns() ; current ++)
    {

        for(int row = _rowStart; row < (MAXROWS + _rowStart); row++)
        {
            drawRow = row + _rowStart;
            past = current + 1;
            color = _ledWrapper->getPixelColor(row, past);          //Get past
            _ledWrapper->setPixelColor(row , current, color);       // move to present
            _ledWrapper->setPixelColor(row , past, off);            //dark hole past

            //   _image[( (drawRow - _rowStart) * _drawCol)]


        }
    }

    _ledWrapper->show();
}



void MatrixScrollText::startShow()
{
    int bitCounter = 0;
    int columnStart = MAXCOLS;


    int drawRow = 0;



  //  while(_endTime > time(nullptr) && _running == true)
  //  {
        for(int letter = 0; letter < _matrixText.length(); letter++)
        {

            if((int)_matrixText.toStdString().c_str()[letter] < 32 || (int)_matrixText.toStdString().c_str()[letter] > 122)
                continue;

            bitCounter = 0;
            for(int col = columnStart; col < (MAXCOLS + columnStart); col++)
            {

                for(int row = 0; row < MAXROWS; row++)
                {
                    drawRow = row + _rowStart;
                    bitCounter = row * MAXCOLS + (col - columnStart);

                    if(letterMatrix[(int)_matrixText.toStdString().c_str()[letter] - 32][bitCounter] == 1)
                    {
                        _ledWrapper->setPixelColor(drawRow, _drawCol , _color1);
                    }
                    else
                    {
                       //_ledWrapper->setPixelColor(drawRow, _drawCol, _image[( (drawRow - _rowStart) * _drawCol)]);
                      _ledWrapper->setPixelColor(drawRow, _drawCol, _ledWrapper->Color(0,0,0));

                 //     qDebug() << "col: " << drawCol << " row: " << drawRow << " imageLoc: " << ( (drawRow - _rowStart) * drawCol) << " colSt: " << columnStart << " maxI: " << MAXROWS * _settings->getStripColumns();

                    }

                }


                _ledWrapper->show();
                 Ws2811Wrapper::waitMillSec(_wait);
                 shiftColumns();

            }

        }


     //   if(_running == false)
      //    break;

  //  }




}


