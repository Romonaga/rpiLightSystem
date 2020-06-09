#include "lightsystem.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>



#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

LightSystem::LightSystem()
{
    _settings = SystemSettings::getInstance();

    _started = false;
    _running = false;
    _red = 0;
    _green = 0;
    _blue = 255;
    _rno = 0x45; // random9 seed
    _cno = 0;       // used to set colours, range 0-0x1FF
    _wait = 200;
    _mqq = nullptr;
    _logger = DNRLogger::instance();
    _logger->setDebugOut(_settings->getDbgLog());
    _logger->setDebugOut(true);
}


LightSystem::~LightSystem()
{
    if(_mqq != nullptr)
    {
        _running = false;
        _started = false;
        _mqq->stop();
        wait();
        _ledWrapper.clearLeds();
    }
}

const char *LightSystem::getEnumName(int index)
{
    const char* enumNames[] = {LIGHT_SHOWS(LIGHT_SHOWS_STRING)};
    return enumNames[index];
}


void LightSystem::processMsgReceived(QString msg)
{
    fprintf(stderr," LightSystem::processMsgReceived: %s\r\n", msg.toStdString().c_str());

    QJsonObject jsonObject;
    QString state;

    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());

    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            jsonObject = doc.object();
            state = jsonObject.value("state").toString();
            _settings->setBrightness(jsonObject.value("brightness").toString().toInt());
            QJsonArray jsonArray = jsonObject["shows"].toArray();

            foreach (const QJsonValue & value, jsonArray)
                _runShows.append(value.toString().toInt());

        }
        else
        {
          //  qDebug() << "Document is not an object" << endl;
            return;
        }
    }
    else
    {
       // qDebug() << "Invalid JSON...\n" << in << endl;

        return;
    }

    _ledWrapper.setBrightness(_settings->getBrightness());

    if(state == "ON")
        start();
    else
    {
         _logger->logInfo("LightSystem::processMsgReceived Stopping Lights");
        _running = false;
        wait();
        _ledWrapper.clearLeds();
        _logger->logInfo("LightSystem::processMsgReceived Lights Stopped");

    }

}

void LightSystem::run()
{
    if(_running) return;

    std::stringstream info;

    _logger->logInfo("LightSystem::run() Started");
    _running = true;

    while(_running)
    {


        if(_runShows.count() > 0)
        {

            info.str("");
            info << "LightSystem::run() Running Show(" << getEnumName(_runShows[0]) << ")";
            _logger->logInfo(info.str());

            switch(_runShows[0])
            {
                case 1:
                    break;

                case 2:

                    //Chaser(_wait);
                    break;

                    case 3:
                        theaterChase(Ws2811Wrapper::Color(_red, _green, _blue, 100), _wait);

                    break;
                    case 4:
                       theaterChaseRainbow(_wait);
                    break;
                    case 5:
                    colorThirdsReverse(_ledWrapper.Color(255, 0, 0), _ledWrapper.Color(255, 255, 255), _ledWrapper.Color(0, 0, 255), _wait);
                    break;
                    case 6:
                    break;
                    case 7:
                         colorWipe(Ws2811Wrapper::Color(_red,_green,_blue,0), _wait);
                    break;
                    case 8:
                    HalfnHalf(Ws2811Wrapper::Color(255,0,0, 255), Ws2811Wrapper::Color(0,0,255,255), _wait);
                    break;
                    case 9:
                     rainbow(_wait);
                    break;
                    case 10:
                     rainbowCycle(_wait);
                    break;

                case 11:
                    NeoRand();
                    break;
                    
                case 12:
                    flame();
                    break;
            }

            info.str("");
            info << "LightSystem::run() Completed Show(" << getEnumName(_runShows[0]) << ")";
            _logger->logInfo(info.str());
             _runShows.removeFirst();
        }

        /*
        _logger->logInfo("LightSystem::run() 1");
        if(_running)
            colorThirdsReverse(_ledWrapper.Color(255, 0, 0), _ledWrapper.Color(255, 255, 255), _ledWrapper.Color(0, 0, 255), _wait);

        _logger->logInfo("LightSystem::run() 2");
        if(_running)
            NeoRand();

        _logger->logInfo("LightSystem::run() 3");
        if(_running)
            flame();

        _logger->logInfo("LightSystem::run() 4");
        if(_running)
            colorWipe(Ws2811Wrapper::Color(_red,_green,_blue,0), _wait);

        //    if(_isRunning)
        //      Chaser(_wait);
        //        if(_isRunning)
        //         HalfnHalf(Ws2811Wrapper::Color(255,0,0, 255), Ws2811Wrapper::Color(0,0,255,255), _wait);

        _logger->logInfo("LightSystem::run() 5");
        if(_running)
            theaterChaseRainbow(_wait);

        _logger->logInfo("LightSystem::run() 6");
        if(_running)
            theaterChase(Ws2811Wrapper::Color(_red, _green, _blue, 100), _wait);

        _logger->logInfo("LightSystem::run() 7");
        if(_running)
            rainbowCycle(_wait);

        _logger->logInfo("LightSystem::run() 8");
        if(_running)
            rainbow(_wait);

         */
    }
}

bool LightSystem::startSystem()
{
    std::stringstream info;
    if(_started ) return true;

    ws2811_return_t renderResults = WS2811_SUCCESS;
    _started = _settings->loadSettings();
    if(_started)
    {
        _logger->logInfo("Setting Up Led Strip");
        info.str("");
        info << "start() Settings Host(" << _settings->getHostName().toStdString().c_str() << ") DMA(" << _settings->getDma() << ") GPIO(" <<
                _settings->getGpio() << ") sType(" << _settings->getStripType() << ") Width(" <<
                _settings->getStripWidth() << ") Height(" << _settings->getStripHeight() << ") Brightness(" <<
                _settings->getBrightness() << ") MQTTBroker(" << _settings->getMqttBroker().toStdString().c_str() << ") DebugLog(" <<
                _settings->getDbgLog() << ")";

       _logger->logInfo(info.str());
       renderResults = _ledWrapper.initStrip(_settings->getStripWidth(), _settings->getStripHeight(), (LedStripType)_settings->getStripType(), _settings->getDma(), _settings->getGpio());
       if(renderResults != WS2811_SUCCESS)
       {
           info.str("");
           info << "LightSystem start() Failed Code(" << renderResults << ") Msg(" <<
                   _ledWrapper.ws2811_get_return_t_str(renderResults) << ")";
           _logger->logWarning(info.str());
           return -12;
       }

        _logger->logInfo("Setting Up MQTT");
        _mqq = new MqttReceiver(_settings->getMqttBroker(), _settings->getHostName(), 1);
        connect(_mqq, SIGNAL(msgReceived(QString)), this, SLOT(processMsgReceived(QString)));


        _logger->logInfo("Configuring LED Strip");
       _ledWrapper.setBrightness(_settings->getBrightness());
       _ledWrapper.setClearOnExit(true);

    }
    else
    {
        _logger->logWarning("Problem Loadng Config File.");
    }
    return _started;
}

void LightSystem::stopSystem()
{
    _running = false;
    _started = false;

}


void LightSystem::theaterChaseRainbow(u_int32_t wait)
{
    _ledWrapper.clearLeds();

    for (int j=0; j < 256; j++)
    {
        if(_running == false)
            return;

        // cycle all 256 colors in the wheel
        for (int q=0; q < 3; q++)
        {
            for (u_int32_t i=0; i < _ledWrapper.getNumberLeds(); i=i+3)
            {
               _ledWrapper.setPixelColor(1, i+q, Ws2811Wrapper::Wheel( (i+j) % 255));    //turn every third pixel on
            }
            _ledWrapper.show();

          Ws2811Wrapper::waitMillSec(wait);
          for (u_int32_t i=0; i < _ledWrapper.getNumberLeds(); i=i+3)
          {
            _ledWrapper.setPixelColor(1, i+q, 0);        //turn every third pixel off
          }
        }
    }
    _ledWrapper.clearLeds();
}

uint32_t LightSystem::Bow(int n)
{

  _cno = int(n / 64);     // find LED colour (0-6)

  _rno = n - (_cno * 64);  // index from LED colour to next colour (0-63)

  switch (_cno) {          // update LED colours
    case 0:   // red
      _red = 255;
      _blue = 0;

      _green = _rno >> 1;   // use shift for div 2
      break;

    case 1:   // orange
      _red = 255;
      _blue = 0;

      _green = 32 + ((_rno * 3) >> 1);
      break;

    case 2:     // yellow
      _red = 255;
      _blue = 0;

      _green = 128 + (_rno * 6);

      if (_green > 255) {
        _red -= (_green - 255);
        _green = 255;
      }
      break;

    case 3:     // green
      _red = 0;
      _green = 255;

      _blue = _rno * 8;

      if (_blue > 255) {
        _green -= (_blue - 255);
        _blue = 255;
      }
      break;

    case 4:     // blue
      _green = 0;
      _blue = 255;

      _red = _rno;
      break;

    case 5:     // indigo
      _green = 0;
      _blue = 255;

      _red = 64 + ((11 * _rno) >> 1);

      if (_red > 255) {
        _blue -= (_red - 255);
        _red = 255;
      }
      break;
    case 6:     // violet
      _red = 255;
      _green = 0;

      _blue = 96 - ((_rno * 3) >> 1);

      break;

    default:                  // should not get here
      _red = 0;
      _green = 0;
      _blue = 0;
  }

    return _ledWrapper.Color(_red, _green, _blue);

}

unsigned int LightSystem::random9()
{

  unsigned int newbit = (((_rno >> 8) ^ (_rno >> 4)) & 1);
  _rno = ((_rno << 1) | newbit) & 0x1FF;
  return _rno;
}

void LightSystem::lightsOff()
{

  for (u_int32_t i = 0; i < _ledWrapper.getNumberLeds(); i++)
  {
    _ledWrapper.setPixelColor(1, i, _ledWrapper.Color(0, 0, 0, 0 ) );
  }
  _ledWrapper.show();
}




void LightSystem::blink(int numBlinks, int wait)
{
  for (int counter = 0; counter < numBlinks; counter++)
  {
    _ledWrapper.setBrightness(1);
    _ledWrapper.show();
     Ws2811Wrapper::waitMillSec(wait);
    _ledWrapper.setBrightness(60);
    _ledWrapper.show();
     Ws2811Wrapper::waitMillSec(wait);
  }
}

void LightSystem::colorThirdsReverse(uint32_t startColor, uint32_t middleColor, uint32_t finishColor, int wait)
{
  uint32_t colors[] = {startColor, middleColor, finishColor};

  if (_running == false) return;


  for(int counter = 0 ; counter <= 2; counter++)
  {
    u_int32_t third = _ledWrapper.getNumberLeds() / 3;
    u_int32_t currentDivision = (third * 2);
    int colorNumber = 1;

    uint32_t currentColor = startColor;

    lightsOff();

    for (u_int32_t counter = (_ledWrapper.getNumberLeds() - 1); counter > 0; counter--)
    {

      if (counter <= currentDivision)
      {
        ++colorNumber;
        currentDivision = (currentDivision - third);
        currentColor = colors[colorNumber - 1];
      }


      _ledWrapper.setPixelColor(1, counter, currentColor);
      _ledWrapper.show();
      if (_running == false) return;

       Ws2811Wrapper::waitMillSec(wait);
    }
    blink(10, 1000);
  }

    _ledWrapper.clearLeds();

}


void LightSystem::NeoRand()
{

  if (_running == false) return;

  u_int32_t tw;
  uint32_t saved = 0;     // saved colour (twinkle)
  unsigned int index;     // used to set colours, range 0-0x1FF
  u_int32_t cnt = 0;        // counter for pixel to set to new colour

  int fade[_ledWrapper.getNumberLeds()];  //fade increment
  int col[_ledWrapper.getNumberLeds()];    // holds colour values, shift>>6 to get index

  memset(&fade, 0, sizeof(fade));
  memset(&col, 0, sizeof(col));

    _rno = random9();
    for(int counter = 0; counter < (255 * 100); counter++)
  {
        if (_running == false) return;
   // rno = random9();
    for (u_int32_t i = 0; i < _ledWrapper.getNumberLeds(); i++)
    {

      if (_ledWrapper.getPixelColor(1, i) == _ledWrapper.Color(255,255,255)) _ledWrapper.setPixelColor(1, i, saved); // return colour after sparkle


      int c = i % 7;          // c=c nominal colour 0-red, 1-orange, etc

      if (i == cnt)
      {
        _cno = random9();      // new colour setting (0 - 0x1FF)
        fade[i] = _cno - (col[i] >> 6); // new fade increment. 64 steps to reach cno
      }

      col[i] += fade[i];      // next setting

      index = col[i] >> 6;    // range 0-0x1FF

      switch (c) {          // update LED colours
        case 0:   // red
          _red = 255;
          _green = 0;
          _blue = 0;
          if (highByte(index) == 0)
          { // 0-0xFF
            _blue = (0xFF - lowByte(index)) >> 1; //max 127
          }
          else {                    // 0x100-0x1FF
            _green = lowByte(index) >> 2; //max 63
          }
          break;

        case 1:   // orange
          _red = 255;
          _green = 32;
          _blue = 0;
          if (highByte(index) == 0) { // 0-0xFF
            _green -= (0xFF - lowByte(index)) >> 3; //1 - 32
          }
          else {                    // 0x100-0x1FF
            _green += lowByte(index) >> 1;       //32 - 140
          }
          break;

        case 2:     // yellow
          _red = 255;
          _green = 127;
          _blue = 0;
          if (highByte(index) == 0) { // 0-0xFF
            _green -= (0xFF - lowByte(index)) >> 4; //64 - 127
          }
          else {                    // 0x100-0x1FF
            _green += lowByte(index);         //127 - 154
            if (_green > 0xFF) {
              _red -= _green - 0xFF;
              _green = 0xFF;
            }
          }
          break;

        case 3:     // green
          _red = 0;
          _green = 255;
          _blue = 0;
          if (highByte(index) == 0) { // 0-0xFF
            _red = 0xFF - lowByte(index); // 255 - 0
          }
          else {                    // 0x100-0x1FF
            _blue += lowByte(index) * 2;       //127 - 154
            if (_blue > 0xFF) {
              _green -= _blue - 0xFF;
              _blue = 0xFF;
            }
          }
          break;

        case 4:     // blue
          _red = 0;
          _green = 0;
          _blue = 255;
          if (highByte(index) == 0) { // 0-0xFF
            _green = (0xFF - lowByte(index)) * 2; // 255 - 0
            if (_green > 0xFF) {
              _blue -= _green - 0xFF;
              _green = 0xFF;
            }
          }
          else {                    // 0x100-0x1FF
            _red = lowByte(index) >> 2;       // red 0-64
          }
          break;

        case 5:     // indigo
          _red = 40;
          _green = 0;
          _blue = 255;
          if (highByte(index) == 0) { // 0-0xFF
            _red -= (0xFF - lowByte(index)) >> 2; // 255 - 0
            if (_red < 0) {
              _green = -_red;
              _red = 0;
            }
          }
          else {                    // 0x100-0x1FF
            _red += lowByte(index) >> 1;       // red 40-255
            if (_red > 0xFF) {
              _blue -= _red - 0xFF;
              _red = 0xFF;
            }
          }
          break;
        case 6:   // violet
          _red = 255;
          _green = 0;
          _blue = 80;
          if (highByte(index) == 0) { // 0-0xFF
            _blue += (0xFF - lowByte(index)) >> 1; // 255 - 0
            if (_blue > 0xFF) {
              _red -= _blue - 0xFF;
              _blue = 0xFF;
            }
          }
          else {                    // 0x100-0x1FF
            _blue -= lowByte(index) >> 1;       // red 40-255
            if (_blue < 0) {
              _green -= _blue - 0xFF;
              _blue = 0;
            }
          }
          break;

        default:                  // should not get here
          _red = 0;
          _green = 0;
          _blue = 0;
      }

      _ledWrapper.setPixelColor(1, i, _ledWrapper.Color(_red, _green, _blue)); // set colour

    }  //  for(int i=0;i<NUMPIXELS;i++){

    cnt += 1;
    if (cnt >= _ledWrapper.getNumberLeds()) cnt = 0;


    tw = _cno - 0xDF;

    if (tw < _ledWrapper.getNumberLeds() && tw == 0 )
    {
      _ledWrapper.setPixelColor(1, tw, _ledWrapper.Color(255,255,255));
    }


    _ledWrapper.show();    // This sends the updated pixel colors to the string.

    Ws2811Wrapper::waitMillSec(_wait);  // Delay (milliseconds).
  }

}

void LightSystem::flame()
{


  //int r = 255, g = 55, b = 0;
  //  Purple flame:
  int r = 165, g = 8, b = 148;

  //  Green flame:
  //int r = 74, g = 150, b = 12;
  //  Flicker, based on our initial RGB values
  for(int counter = 0; counter < 255; counter++)
  {
      if(_running == false)
          return;// int r = 255, g = 50, b = 35;

    for (u_int32_t i = 0; i < _ledWrapper.getNumberLeds(); i++)
    {
      int flicker = (rand() % (100 - 0 + 1)) + 0;
      int r1 = r - flicker;
      int g1 = g - flicker;
      int b1 = b - flicker;
      if (g1 < 0) g1 = 0;
      if (r1 < 0) r1 = 0;
      if (b1 < 0) b1 = 0;
      _ledWrapper.setPixelColor(1, i, r1, g1, b1);
    }
    _ledWrapper.show();


    //  Adjust the delay here, if you'd like.  Right now, it randomizes the
    //  color switch delay to give a sense of realism
    Ws2811Wrapper::waitMillSec((rand() % (150 - 20 + 1)) + 20);
  }
  _ledWrapper.clearLeds();
}

void LightSystem::Chaser(u_int32_t wait)
{
    ws2811_return_t renderResults;

    _ledWrapper.clearLeds();

    for(u_int32_t counter = 0; counter < _ledWrapper.getNumberLeds(); counter++)
    {
        _ledWrapper.setPixelColor(_settings->getStripWidth(), counter, _red, _green , _blue);
        renderResults = _ledWrapper.show();
        if(renderResults != WS2811_SUCCESS)
        {
            fprintf(stderr, "Render Failed Code(%d) Msg(%s)", renderResults, _ledWrapper.ws2811_get_return_t_str(renderResults));
            return;
        }

        Ws2811Wrapper::waitMillSec(wait);

    }
    _ledWrapper.clearLeds();
}

void LightSystem::theaterChase(ws2811_led_t baseColor, u_int32_t wait)
{
    _ledWrapper.clearLeds();
    for (int j=0; j<10; j++)
    {  //do 10 cycles of chasing
        for (int q=0; q < 3; q++)
        {
            for (u_int32_t i=0; i < _ledWrapper.getNumberLeds(); i=i+3)
            {
                _ledWrapper.setPixelColor(1, i+q, baseColor);    //turn every third pixel on
            }
            _ledWrapper.show();
            Ws2811Wrapper::waitMillSec(wait);

        for (u_int32_t i=0; i < _ledWrapper.getNumberLeds(); i=i+3)
        {
            _ledWrapper.setPixelColor(1, i+q, 0);        //turn every third pixel off
      }
    }
  }
  _ledWrapper.clearLeds();
}

void LightSystem::cylon(ws2811_led_t c, int width, int speed)
{
  // First slide the leds in one direction
  for(u_int32_t i = 0; i <= _ledWrapper.getNumberLeds() - width; i++)
  {
    for(int j=0; j<width; j++)
    {
        _ledWrapper.setPixelColor(1, i + j, c);
    }

    _ledWrapper.show();

    // now that we've shown the leds, reset to black for next loop
    for(int j=0; j<5; j++)
    {
        _ledWrapper.setPixelColor(1, i + j, _ledWrapper.Color(0,0,0));
      //leds[i+j] = CRGB::Black;
    }
    Ws2811Wrapper::waitMillSec(speed);
  }

  // Now go in the other direction.
  for(u_int32_t i = _ledWrapper.getNumberLeds() - width; i == 0; i--)
  {
    for(int j=0; j<width; j++)
    {
    _ledWrapper.setPixelColor(1, i + j, c);
    }
    _ledWrapper.show();
    for(int j=0; j<width; j++)
    {
       _ledWrapper.setPixelColor(1, i + j, _ledWrapper.Color(0,0,0));
    }

    Ws2811Wrapper::waitMillSec(speed);
  }

  _ledWrapper.clearLeds();
}


void LightSystem::colorWipe(ws2811_led_t color, u_int32_t waitms)
{
  _ledWrapper.clearLeds();

  for(uint32_t i=0; i < _ledWrapper.getNumberLeds(); i++)
  {
    _ledWrapper.setPixelColor(1, i, color);
    Ws2811Wrapper::waitMillSec(waitms);
    _ledWrapper.show();
    Ws2811Wrapper::waitMillSec(waitms);

  }
    _ledWrapper.clearLeds();
}

void LightSystem::HalfnHalf(ws2811_led_t halfN, ws2811_led_t nHalf, u_int32_t delayMs)
{
    u_int32_t half = _ledWrapper.getNumberLeds() / 2;
    u_int32_t counter = 0;

     _ledWrapper.clearLeds();
    for(counter = 0; counter < half; counter++)
    {
        _ledWrapper.setPixelColor(_settings->getStripWidth(), counter, halfN);
        _ledWrapper.show();
        Ws2811Wrapper::waitMillSec(delayMs);

    }

    for(; counter < _ledWrapper.getNumberLeds(); counter++)
    {
        _ledWrapper.setPixelColor(_settings->getStripWidth(), counter, nHalf);
        _ledWrapper.show();
        Ws2811Wrapper::waitMillSec(delayMs);
    }
_ledWrapper.clearLeds();
}

void LightSystem::rainbow(u_int32_t wait)
{
  u_int32_t i, j;

  _ledWrapper.clearLeds();

  for(j=0; j < 256; j++)
  {
    if(_running == false)
        return;

    for(i=0; i <_ledWrapper.getNumberLeds(); i++)
    {
      _ledWrapper.setPixelColor(1, i,  Ws2811Wrapper::Wheel((i+j) & 255));
    }
    _ledWrapper.show();
    Ws2811Wrapper::waitMillSec(wait);

  }
    _ledWrapper.clearLeds();
}

void LightSystem::rainbowCycle(u_int32_t wait)
{
    u_int32_t i, j;

    for(j=0; j <256 * 5; j++)
    {
        if(_running == false)
            return;

        // 5 cycles of all colors on wheel
        for(i=0; i < _ledWrapper.getNumberLeds(); i++)
           _ledWrapper.setPixelColor(1, i, Ws2811Wrapper::Wheel(((i * 256 / _ledWrapper.getNumberLeds()) + j) & 255));

        _ledWrapper.show();
        Ws2811Wrapper::waitMillSec(wait);
    }
    _ledWrapper.clearLeds();
}
