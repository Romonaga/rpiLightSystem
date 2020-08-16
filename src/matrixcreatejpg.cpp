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
    char tmpFile[] = "/art_XXXXXX";
    int tmp = 0;

    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;
    ws2811_led_t color = 0;
    unsigned int bufferIndex = 0;

    ws2811_led_t* snapShotBuffer = nullptr;
    unsigned char* reSampled = nullptr;
    uint32_t snapShotbufferSize = 0;

    snapShotBuffer = snapShot(0, _settings->getChannels()[_channelId]->stripRows(), &snapShotbufferSize);
    unsigned char* imageData = new unsigned char[snapShotbufferSize * 3];


    for(unsigned int counter = 0; counter < _ledWrapper->getNumberLeds(); counter++)
    {
        color = snapShotBuffer[counter];

        red =  _ledWrapper->Red(color);
        imageData[bufferIndex++] = red;

        green =  _ledWrapper->Green(color);
        imageData[bufferIndex++] = green;

        blue =  _ledWrapper->Blue(color);
        imageData[bufferIndex++] = blue;
    }

    delete [] snapShotBuffer;
    reSampled = resampleRGB(_settings->getChannels()[_channelId]->stripColumns() * 20,_settings->getChannels()[_channelId]->stripRows() * 20, _settings->getChannels()[_channelId]->stripColumns(), _settings->getChannels()[_channelId]->stripRows(), imageData);
    delete [] imageData;

    if(reSampled != nullptr)
    {
        auto jpgConverter = [](void* caller, unsigned char byte)
        {
            ((MatrixCreateJpg*)caller)->_jpgBuffer.push_back(byte);
        };

        TooJpeg::writeJpeg(this, jpgConverter, reSampled, _settings->getChannels()[_channelId]->stripColumns() * 20 ,_settings->getChannels()[_channelId]->stripRows() * 20, true, 100, false, nullptr);

        if(reSampled != nullptr)
        {
            tmp = mkstemp(tmpFile);
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

            delete [] reSampled;
        }
    }

    _jpgBuffer.clear();

}
