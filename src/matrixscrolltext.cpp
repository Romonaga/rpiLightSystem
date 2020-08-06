#include "matrixscrolltext.h"
#include <QVector>
#include <math.h>
#include <QDebug>
#include "font7x6ext.h"


MatrixScrollText::MatrixScrollText(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

    _rowStart =  (_settings->getChannels()[_channelId]->stripRows()/ 2) - (MAXROWS / 2) - 1;
    _drawCol = _settings->getChannels()[_channelId]->stripColumns() - 1;

    _image = nullptr;

}

MatrixScrollText::~MatrixScrollText()
{
    if(_image != nullptr)
        delete [] _image;
}

//logic is clear, but we are walking from where we drew the row
//and we are moving that row forward, and preserving the original color, if it is not a letter's color
void MatrixScrollText::shiftColumns()
{
    ws2811_led_t drawColor;

    int past = 0;
    for(int current = 0; current  < _settings->getChannels()[_channelId]->stripColumns() - 1 ; current ++)
    {

        for(int row = _rowStart; row < (MAXROWS + _rowStart); row++)
        {
            past = current + 1;

            drawColor = _ledWrapper->getPixelColor(row, past);                                                              //init drawColor to past color

            if( (drawColor != _color1 && _ledWrapper->getPixelColor(row, current) != _color1) )                             //replay saved color if needed
                drawColor = _image[( (row - _rowStart) * _settings->getChannels()[_channelId]->stripColumns()) + current];

            _ledWrapper->setPixelColor(row , current, drawColor);                                                           // move to present
            _ledWrapper->setPixelColor(row , past, _image[( (row - _rowStart) * _settings->getChannels()[_channelId]->stripColumns()) + past]);     //set  past's past color to saved color (yes, I know)..


        }
    }

    _ledWrapper->show();
}

//all we are doing here is captruing our drawing area.  As it is defined,
// we dont need to deal with the complete matrix..  Just what we touch.
void MatrixScrollText::snapShot()
{

    //We only care about what we might overlay.
    _image = new ws2811_led_t[MAXROWS * _settings->getChannels()[_channelId]->stripColumns()];

    for(int col = 0; col < _settings->getChannels()[_channelId]->stripColumns(); col++)
    {
        for(int row = 0; row < MAXROWS ; row++)
            _image[ (row  * _settings->getChannels()[_channelId]->stripColumns()) + col]  = _ledWrapper->getPixelColor(row + _rowStart, col);
    }
}

//This will reprint the snapshot so to speak.  Here we will
//simply get the picture and repaint it, to put the picture back to how
//it was before we touched it
void MatrixScrollText::replaySnapShot()
{
    for(int col = 0; col < _settings->getChannels()[_channelId]->stripColumns(); col++)
    {
        for(int row = 0; row < MAXROWS; row++)
            _ledWrapper->setPixelColor(row + _rowStart, col, _image[ (row  * _settings->getChannels()[_channelId]->stripColumns()) + col]);

    }

    _ledWrapper->show();

    delete [] _image;
    _image = nullptr;
}

void MatrixScrollText::scrollText(QString msg, bool noDelay)
{


    for(int letter = 0; letter < msg.length(); letter++)
    {

        if((int)msg.toStdString().c_str()[letter] < 32 || (int)msg.toStdString().c_str()[letter] > 122) //only attempt to print what we know.
            continue;


        for(int col = MAXCOLS; col < (MAXCOLS * 2); col++)
        {
            for(int row = 0; row < MAXROWS; row++)
            {

                if(letterMatrix[(int)msg.toStdString().c_str()[letter] - 32][row * MAXCOLS + (col - MAXCOLS)] == 1) //should this pixal be on?
                    _ledWrapper->setPixelColor(row + _rowStart, _drawCol , _color1);
            }


            _ledWrapper->show();

            if(noDelay == false)
                Ws2811Wrapper::waitMillSec(_wait);

            shiftColumns();

        }
    }

}


void MatrixScrollText::startShow()
{

    QString sendPad;

    if(_matrixText.length() == 0)
        return;

    bool fastPad = (bool)_showParmsJson.value("fastPad").toInt();
    for(int pad = 0; pad < _settings->getChannels()[_channelId]->stripColumns() / MAXCOLS; pad++)       //pad the string so it will scroll off the screen
        sendPad.append(" ");

    if(fastPad == false)
        _matrixText.append(sendPad);

    snapShot();

    while(_endTime > time(nullptr) && _running == true)
    {

        scrollText(_matrixText, !fastPad);

        if(fastPad == true)
            scrollText(sendPad, fastPad);
    }



    replaySnapShot();

}


