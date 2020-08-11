#include "matrixscrolltext.h"
#include <QVector>
#include <math.h>
#include <QDebug>



MatrixScrollText::MatrixScrollText(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

    _rowStart =  (_settings->getChannels()[_channelId]->stripRows()/ 2) - (MAXROWS / 2) - 1;
    _drawCol = _settings->getChannels()[_channelId]->stripColumns() - 1;

}

MatrixScrollText::~MatrixScrollText()
{

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

       if(_showParmsJson.value("position").isString())
       {

           switch(_showParmsJson.value("position").toString().toInt())
           {

           case 1:
                _rowStart = 0;
               break;

           case 2:
                _rowStart =  (_settings->getChannels()[_channelId]->stripRows() / 2) - (MAXROWS / 2) - 1;
               break;

           case 3:
               _rowStart = _settings->getChannels()[_channelId]->stripRows() - MAXROWS;
               break;


           }
       }

       snapShot();

       while(_endTime > time(nullptr) && _running == true)
       {

           if(fastPad == true)
           {
               scrollText(_matrixText, true);
               scrollText(sendPad, false);
           }
           else
           {
               scrollText(_matrixText, false);
           }
       }

       replaySnapShot();


}


