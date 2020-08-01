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
            //qDebug() << "col: " << col << " row: " << (row - _rowStart) << " rowStart: " << _rowStart << " imageLoc: " << ( (row - _rowStart) * _settings->getStripColumns()) + col << " MaxLoc: " << MAXROWS * _settings->getStripColumns();
            _image[( (row - _rowStart) * _settings->getStripColumns()) + col] = _ledWrapper->getPixelColor(row, col);
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
    ws2811_led_t blackColor = _ledWrapper->Color(0,0,0);

    int past = 0;

    for(int current = 0; current  < _settings->getStripColumns() - 1 ; current ++)
    {

        for(int row = _rowStart; row < (MAXROWS + _rowStart); row++)
        {
            past = current + 1;

            color = _ledWrapper->getPixelColor(row, past);          //Get past
            _ledWrapper->setPixelColor(row , current, color);       // move to present
            //_ledWrapper->setPixelColor(row , past, blackColor);     // black hole the past for now.


         //   if(_image[( (row - _rowStart) * _settings->getStripColumns()) + current] != _color1)
       //         _ledWrapper->setPixelColor(row , current, _image[( (row - _rowStart) * _settings->getStripColumns()) + current]);    //set to past past colors color
             _ledWrapper->setPixelColor(row , past, _image[( (row - _rowStart) * _settings->getStripColumns()) + past]);    //set to past past colors color

            //    qDebug() << "shiftColumns-- col: " << past << " row: " << (row - _rowStart) << " rowStart: " << _rowStart << " imageLoc: " << ( (row - _rowStart) * _settings->getStripColumns()) + past << " MaxLoc: " << MAXROWS * _settings->getStripColumns();


//            _ledWrapper->setPixelColor(row , past, _image[( (row - _rowStart) * _settings->getStripColumns()) + past]);    //set to past past colors color

            //    qDebug() << "col: " << past << " row: " << (row - _rowStart) << " rowStart: " << _rowStart << " imageLoc: " << ( (row - _rowStart) * _settings->getStripColumns()) + past << " MaxLoc: " << MAXROWS * _settings->getStripColumns();



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
                   //    _ledWrapper->setPixelColor(drawRow, _drawCol, _image[( row * _settings->getStripColumns()) + col - columnStart]);
                      _ledWrapper->setPixelColor(drawRow, _drawCol, _ledWrapper->Color(0,0,0));

                      qDebug() << "startShow-- col: " << col - columnStart << " row: " << row << " rowStart: " << _rowStart << " imageLoc: " << ( row * _settings->getStripColumns()) + col - columnStart  << " MaxLoc: " << MAXROWS * _settings->getStripColumns();


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


