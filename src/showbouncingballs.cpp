#include "showbouncingballs.h"

#include <QDebug>
//#include <portaudiocpp/PortAudioCpp.hxx>


ShowBouncingBalls::ShowBouncingBalls(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}

ShowBouncingBalls::~ShowBouncingBalls()
{

}

void ShowBouncingBalls::startShow()
{
    /*
    portaudio::System::initialize();
    auto& sys = portaudio::System::instance();
    auto& dev = sys.defaultInputDevice();
    qDebug() << dev.name();

    auto def_sr = dev.defaultSampleRate();
    constexpr float samplerate = 16000;

    qDebug() << "Sr: " << def_sr ;
    portaudio::DirectionSpecificStreamParameters inParams(dev, 1, portaudio::FLOAT32, true, dev.defaultLowInputLatency(), nullptr);
    portaudio::StreamParameters params(inParams, portaudio::DirectionSpecificStreamParameters::null(), def_sr, (int)def_sr, paClipOff);

    portaudio::BlockingStream bs(params);


    bs.start();


    qDebug() << "isOpen: " << bs.isOpen() << " active: " << bs.isActive();
    float buf[1024];
    while (_running)
    {

        try
        {

            bs.read(buf, 1024);

            qDebug() << "buf[0]: " << buf[0];

        }
        catch(portaudio::PaException pe)
        {
            qDebug() << "error: " << pe.what();
        }

        //qDebug() << "buff0: " << buf[0];
    }


    bs.close();
    */
}


/*
 *
 * portaudio::System::initialize();
    auto& sys = portaudio::System::instance();
    auto& dev = sys.defaultInputDevice();
    auto def_sr = dev.defaultSampleRate();
    constexpr float samplerate = 16000;

    portaudio::DirectionSpecificStreamParameters inParams(dev, 1, portaudio::FLOAT32, true, dev.defaultLowInputLatency(), nullptr);
    portaudio::StreamParameters params(inParams, portaudio::DirectionSpecificStreamParameters::null(), samplerate, (int)samplerate, paClipOff);

    portaudio::BlockingStream bs(params);


    bs.start();

    float buf[1024];
    fmt::print("steam: open: {}, active: {}", bs.isOpen(), bs.isActive())
    while (true)
    {
        bs.read(buf, 1024);
        fmt.print("first byte, always 0 if I don't have access: {}", buf[0]);
    }
    return 0

    */
