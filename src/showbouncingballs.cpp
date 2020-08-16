#include "showbouncingballs.h"
#include <QVector>
#include <QFileInfo>
#include <math.h>
#include <QDebug>

extern "C"
{
    #include "gifdec.h"
}


using namespace std;


ShowBouncingBalls::ShowBouncingBalls(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}


void ShowBouncingBalls::startShow()
{


}


/*

void ShowBouncingBalls::startShow()
{
    int width = 0;
    int height = 0;
    int comps = 3;
    int index = 0;


//    unsigned char* data = jpgd::decompress_jpeg_image_from_file("/home/hellweek/code/userArt/art_Hk0cN5.jpg", &width, &hight, &comps, 3 );
//    qDebug() << "w: " << width << " h: " << hight;

//    unsigned char* image =  resample(_settings->getChannels()[_channelId]->stripColumns(),_settings->getChannels()[_channelId]->stripRows(), width, hight, data);


//    for(int row = 0; row < _settings->getChannels()[_channelId]->stripRows(); row++)
//    {

//        for(int col = 0; col < _settings->getChannels()[_channelId]->stripColumns(); col++)
//        {


//            _ledWrapper->setPixelColor(row, col, image[index], image[index + 1], image[index + 2] );
//            index += 3;

//            //_ledWrapper->setPixalColor(row, col, image[index++]);

//        }
//    }

//    _ledWrapper->show();
//    delete [] data;
//    delete [] image;


    unsigned error;
    unsigned char* image1 = nullptr;
    unsigned width1, height1 = 0;

    error = lodepng_decode24_file(&image1, &width1, &height1, "/home/hellweek/code/userArt/greenthing.png");
    unsigned char* image =  resample(_settings->getChannels()[_channelId]->stripColumns(),_settings->getChannels()[_channelId]->stripRows(), width1, height1, image1);
    free(image1);

  //  qDebug() << "grwol width: " << width1 << " h: " << height1 << " error: " << error << " buf size: " << bufferSize << "my calc: " << (width1 * height1) * 3;

    index = 0;
    for(int row = 0; row < _settings->getChannels()[_channelId]->stripRows(); row++)
    {

        for(int col = 0; col < _settings->getChannels()[_channelId]->stripColumns(); col++)
        {


            _ledWrapper->setPixelColor(row, col, image[index], image[index + 1], image[index + 2] );
            index += 3;

            //_ledWrapper->setPixalColor(row, col, image[index++]);

        }
    }

    _ledWrapper->show();


}


*/
