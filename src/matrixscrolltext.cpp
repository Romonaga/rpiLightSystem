#include "matrixscrolltext.h"
#include <QVector>
#include <math.h>
#include <QDebug>
#include "font7x6ext.h"

MatrixScrollText::MatrixScrollText(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

    //We only care about what we might overlay.
    _image = new ws2811_led_t[MAXROWS * _settings->getStripColumns()];

    _rowStart =  (_settings->getStripRows() / 2) - (MAXROWS / 2) - 1;
    _drawCol = _settings->getStripColumns() - 1;

    //** WARNING picture time... SNAP!
    for(int col = 0; col < _settings->getStripColumns(); col++)
    {
        for(int row = _rowStart; row < (MAXROWS + _rowStart); row++)
        {
            _image[( (row - _rowStart) * _settings->getStripColumns()) + col] = _ledWrapper->getPixelColor(row, col);
        }
    }

}

MatrixScrollText::~MatrixScrollText()
{
    delete [] _image;
}

//logic is clear, but we are walking from where we drew the row
//and we are moving that row forward, and preserving the original color, if it is not a letter's color
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

            drawColor = _ledWrapper->getPixelColor(row, past);                                                              //init drawColor to past color
            currentColor = _ledWrapper->getPixelColor(row, current);                                                        //get current color

            if( (drawColor != _color1 && currentColor != _color1) )                                                         //replay saved color if needed
                drawColor = _image[( (row - _rowStart) * _settings->getStripColumns()) + current];

            _ledWrapper->setPixelColor(row , current, drawColor);                                                           // move to present
            _ledWrapper->setPixelColor(row , past, _image[( (row - _rowStart) * _settings->getStripColumns()) + past]);     //set  past's past color to saved color (yes, I know)..
        }
    }

    _ledWrapper->show();
}


//Yes, I know more math.  Trust me, I wrote the damn code, lots of logging, lots of paper!
//Not much to say here code says it all. but this is where we write to the last secten as centerd to the grid as I can get
void MatrixScrollText::startShow()
{

    for(int pad = 0; pad < _settings->getStripColumns() / MAXCOLS; pad++) //pad the string so it will scroll off the screen
        _matrixText.append(" ");

    while(_endTime > time(nullptr) && _running == true)
    {
        for(int letter = 0; letter < _matrixText.length(); letter++)
        {

            if((int)_matrixText.toStdString().c_str()[letter] < 32 || (int)_matrixText.toStdString().c_str()[letter] > 122) //only attempt to print what we know.
                continue;


            for(int col = MAXCOLS; col < (MAXCOLS * 2); col++)
            {
                for(int row = 0; row < MAXROWS; row++)
                {

                    if(letterMatrix[(int)_matrixText.toStdString().c_str()[letter] - 32][row * MAXCOLS + (col - MAXCOLS)] == 1) //should this pixal be on?
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


