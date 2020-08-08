#include "matrixscrolltext.h"
#include <QVector>
#include <math.h>
#include <QDebug>



MatrixScrollText::MatrixScrollText(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


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


