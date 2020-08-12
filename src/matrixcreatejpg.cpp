#include "matrixcreatejpg.h"
#include <QVector>
#include <math.h>
#include <QDebug>
#include <fstream>
#include <unistd.h>

#include "toojpeg.h"




MatrixCreateJpg::MatrixCreateJpg(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}



void MatrixCreateJpg::startShow()
{
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;
    ws2811_led_t color = 0;

    snapShot(0, _settings->getChannels()[_channelId]->stripRows());

    unsigned char* imageData = new unsigned char[getSnapShotBufferSize() * 3];


    unsigned int bufferIndex = 0;

    for(unsigned int counter = 0; counter < _ledWrapper->getNumberLeds(); counter++)
    {
        color = _image[counter];

        red =  _ledWrapper->Red(color);
        imageData[bufferIndex++] = red;

        green =  _ledWrapper->Green(color);
        imageData[bufferIndex++] = green;

        blue =  _ledWrapper->Blue(color);
        imageData[bufferIndex++] = blue;
    }

    auto jpgConverter = [](void* caller, unsigned char byte)
    {
        ((MatrixCreateJpg*)caller)->_jpgBuffer.push_back(byte);
    };


     unsigned char* reSampled = resample(_settings->getChannels()[_channelId]->stripColumns() * 20,_settings->getChannels()[_channelId]->stripRows() * 20, _settings->getChannels()[_channelId]->stripColumns(), _settings->getChannels()[_channelId]->stripRows(), imageData);
    TooJpeg::writeJpeg(this, jpgConverter, reSampled, _settings->getChannels()[_channelId]->stripColumns() * 20 ,_settings->getChannels()[_channelId]->stripRows() * 20, true, 100, false, nullptr);
    //TooJpeg::writeJpeg(this, jpgConverter, imageData, _settings->getChannels()[_channelId]->stripColumns(), _settings->getChannels()[_channelId]->stripRows(), true, 100, false, nullptr);


    char tmpFile[] = "/art_XXXXXX";
    int tmp = mkstemp(tmpFile);
    close(tmp);

    QString outFileName(_settings->getUserArtDirectory());
    outFileName.append(tmpFile);
    outFileName.append(".jpg");

    qDebug() << outFileName.toStdString().c_str();
    std::ofstream outFile(outFileName.toStdString().c_str(), std::ios::binary );
    if(outFile.is_open())
    {
        outFile.write((const char*) _jpgBuffer.data(), _jpgBuffer.size());
        outFile.close();
    }

    deleteSnapShot();
    delete [] imageData;
    delete [] reSampled;

}