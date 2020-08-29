#include "matrixscrolltext.h"
#include <QVector>
#include <math.h>
#include <QDebug>



MatrixScrollText::MatrixScrollText(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

    _rowStart =  (_settings->getChannels()[_channelId]->stripRows()/ 2) - (TEXTMAXROWS / 2) - 1;
    _drawCol = _settings->getChannels()[_channelId]->stripColumns() - 1;

}



MatrixScrollText::~MatrixScrollText()
{

}


void MatrixScrollText::startShow()
{

    if(_settings->getChannels()[_channelId]->matrixdirection() == 0) return;


    QString sendPad;
    uint32_t snapshotBufferSize;
    ws2811_led_t* snapShotBuffer = nullptr;

    if(_matrixText.length() == 0)  return;

    bool fastPad = (bool)_showParmsJson.value("fastPad").toInt();

    for(int pad = 0; pad < _settings->getChannels()[_channelId]->stripColumns() / TEXTMAXCOLS; pad++)       //pad the string so it will scroll off the screen
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
           _rowStart =  (_settings->getChannels()[_channelId]->stripRows() / 2) - (TEXTMAXROWS / 2) - 1;
        break;

        case 3:
           _rowStart = _settings->getChannels()[_channelId]->stripRows() - TEXTMAXROWS;
        break;


        }
    }

    snapShotBuffer = snapShot(_rowStart, TEXTMAXROWS, _settings->getChannels()[_channelId]->stripColumns(), &snapshotBufferSize);
    if(snapShotBuffer != nullptr)
    {
        while(_endTime > time(nullptr) && _running == true)
        {

            if(fastPad == true)
            {
                scrollText(_matrixText, TEXTMAXROWS, TEXTMAXCOLS, _rowStart, _drawCol, _color1, snapShotBuffer, _wait);
                scrollText(sendPad, TEXTMAXROWS, TEXTMAXCOLS, _rowStart, _drawCol, _color1, snapShotBuffer, 0);
            }
            else
            {
                scrollText(_matrixText, TEXTMAXROWS, TEXTMAXCOLS, _rowStart, _drawCol, _color1, snapShotBuffer, _wait);
            }
        }

        replaySnapShot(_rowStart, TEXTMAXROWS, _settings->getChannels()[_channelId]->stripColumns(), snapShotBuffer);
        delete [] snapShotBuffer;
    }

}


