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
            _image[( (row - _rowStart) * _settings->getStripColumns()) + col] = _ledWrapper->getPixelColor(row, col);
            //qDebug() << "col: " << col << " row: " << (row - _rowStart) << " rowStart: " << _rowStart << " imageLoc: " << ( (row - _rowStart) * _settings->getStripColumns()) + col << " MaxLoc: " << MAXROWS * _settings->getStripColumns();

        }
    }

}

MatrixScrollText::~MatrixScrollText()
{
    delete [] _image;
}

void MatrixScrollText::shiftColumns()
{
    ws2811_led_t drawColor;
    ws2811_led_t currentColor;

    int past = 0;
    for(int current = 0; current  < _settings->getStripColumns() - 1 ; current ++)
    {

        for(int row = _rowStart; row < (MAXROWS + _rowStart); row++)
        {
            past = current + 1;

            drawColor = _ledWrapper->getPixelColor(row, past);          //Get past color
            currentColor = _ledWrapper->getPixelColor(row, current);    //get current color

            if( (drawColor != _color1 && currentColor != _color1) || past == 0)
                drawColor = _image[( (row - _rowStart) * _settings->getStripColumns()) + current];  //replay saved color

            _ledWrapper->setPixelColor(row , current, drawColor);       // move to present
            _ledWrapper->setPixelColor(row , past, _image[( (row - _rowStart) * _settings->getStripColumns()) + past]);    //set to past past colors color

            //qDebug() << "past: " <<     (int) pastColor << " current: " << (int) currentColor << " mapColor: " << (int)mapColor << " color1: " << _color1;
        }
    }

    _ledWrapper->show();
}



void MatrixScrollText::startShow()
{

    for(int pad = 0; pad < _settings->getStripColumns() / MAXCOLS; pad++)
        _matrixText.append(" ");

    while(_endTime > time(nullptr) && _running == true)
    {
        for(int letter = 0; letter < _matrixText.length(); letter++)
        {

            if((int)_matrixText.toStdString().c_str()[letter] < 32 || (int)_matrixText.toStdString().c_str()[letter] > 122)
                continue;

            for(int col = MAXCOLS; col < (MAXCOLS * 2); col++)
            {
                for(int row = 0; row < MAXROWS; row++)
                {

                    if(letterMatrix[(int)_matrixText.toStdString().c_str()[letter] - 32][row * MAXCOLS + (col - MAXCOLS)] == 1)
                        _ledWrapper->setPixelColor(row + _rowStart, _drawCol , _color1);
                }

                _ledWrapper->show();
                 Ws2811Wrapper::waitMillSec(_wait);
                 shiftColumns();

            }

        }

        shiftColumns();
        if(_running == false)
          break;

    }

}


