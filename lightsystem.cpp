#include "lightsystem.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include "showfire.h"



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
    _rno = rand();
    _cno = 0;       // used to set colours, range 0-0x1FF
    _wait = 10;
    _mqq = nullptr;
    _logger = DNRLogger::instance();
    _logger->setDebugOut(_settings->getDbgLog());

}


LightSystem::~LightSystem()
{
    if(_mqq != nullptr)
    {
        _running = false;
        _started = false;
        _mqq->stop();
        wait();

    }
    _ledWrapper.clearLeds();
}

const char *LightSystem::getEnumName(int index)
{
    const char* _lightShowNames[] = {LIGHT_SHOWS(LIGHT_SHOWS_STRING)};

    return _lightShowNames[index];
}


void LightSystem::stopLights()
{
    _logger->logInfo("LightSystem::processMsgReceived Stopping Lights");
   _running = false;
   wait();
   _ledWrapper.clearLeds();
   _logger->logInfo("LightSystem::processMsgReceived Lights Stopped");
}

void LightSystem::processMsgReceived(QString msg)
{
   // fprintf(stderr," LightSystem::processMsgReceived: %s\r\n", msg.toStdString().c_str());

    QJsonObject jsonObject;
    QString state;

    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());

    std::stringstream info;


    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            jsonObject = doc.object();
            state = jsonObject.value("state").toString();
            _settings->setBrightness(jsonObject.value("brightness").toString().toInt());
            QJsonArray jsonArray = jsonObject["shows"].toArray();

            foreach (const QJsonValue & value, jsonArray)
            {
                _runShows.append(value.toString().toInt());
                info.str("");
                info << "LightSystem::processMsgReceived adding Show(" << getEnumName(value.toString().toInt()) << ")";
                _logger->logInfo(info.str());

            }
        }
        else
        {
            _logger->logWarning("LightSystem::processMsgReceived Document is not an object");
            return;
        }
    }
    else
    {
        _logger->logWarning("LightSystem::processMsgReceived Invalid JSON");
        return;
    }

    _ledWrapper.setBrightness(_settings->getBrightness());

    if(state == "ON")
        start();
    else
        stopLights();

}


void LightSystem::run()
{
    if(_running) return;

    this->setPriority(QThread::TimeCriticalPriority);
    std::stringstream info;


    _logger->logInfo("LightSystem::run() Started");
    _running = true;

    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);

    while((_conditionVar.wait_for(lck,std::chrono::milliseconds(10)) == std::cv_status::timeout))
    {
        if(false == _running) return;

        if(_runShows.count() > 0)
        {

            info.str("");
            info << "LightSystem::run() Running Show(" << getEnumName(_runShows[0]) << ")";
            _logger->logInfo(info.str());

            switch(_runShows[0])
            {
                case Blink:
                break;

                case Chaser:
                    chaser(_wait);
                break;

                case TC:
                    theaterChase(Ws2811Wrapper::Color(_red, _green, _blue, 100), _wait);
                break;

                case TCR:
                   theaterChaseRainbow(_wait);
                break;

                case Color3R:
                    colorThirdsReverse(Ws2811Wrapper::Color(255, 0, 0), Ws2811Wrapper::Color(255, 255, 255), Ws2811Wrapper::Color(0, 0, 255), _wait);
                break;

                case Cylon:
                    cyclon(Ws2811Wrapper::Color(125, 50, 145), 20, 30);
                break;

                case ColorWipe:
                    colorWipe(Ws2811Wrapper::Color(_red,_green,_blue,0), _wait);
                break;

                case HAndH:
                    halfnHalf(Ws2811Wrapper::Color(255,0,0, 255), Ws2811Wrapper::Color(0,0,255,255), _wait);
                break;

                case Rainbow:
                    rainbow(_wait);
                break;

                case RainbowCycle:
                    rainbowCycle(_wait);
                break;

                case NeoRand:
                    neoRand();
                break;
                    
                case Flame:
                    flame();
                    break;

                case ColorThirds:
                    colorThirds(Ws2811Wrapper::Color(255,120,50), Ws2811Wrapper::Color(50,120,255), Ws2811Wrapper::Color(120,255,50), _wait);
                break;

                case ColorForths:
                    colorForths(Ws2811Wrapper::Color(255,120,50), Ws2811Wrapper::Color(50,120,255), Ws2811Wrapper::Color(120,255,50), Ws2811Wrapper::Color(0,255,50), _wait);
                break;

                case TriChaser:
                    triChaser(Ws2811Wrapper::Color(255, 0, 0), Ws2811Wrapper::Color(255, 255, 255), Ws2811Wrapper::Color(0, 0, 255), _wait);
                break;


            }

            info.str("");
            info << "LightSystem::run() Completed Show(" << getEnumName(_runShows[0]) << ")";
            _logger->logInfo(info.str());
             _runShows.removeFirst();
        }


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
     _conditionVar.notify_all();
    _running = false;
    _started = false;

}

void LightSystem::theaterChase(ws2811_led_t baseColor, u_int32_t wait)
{
    _ledWrapper.clearLeds();
    for (int j=0; j < 256; j++)
    {  //do 10 cycles of chasing
        for (int q=0; q < 3; q++)
        {
            for (u_int32_t i=0; i < _ledWrapper.getNumberLeds(); i=i+3)
            {
                _ledWrapper.setPixelColor(_settings->getStripHeight(), i+q, baseColor);    //turn every third pixel on
            }
            _ledWrapper.show();
            if(_running == false)
                return;
            Ws2811Wrapper::waitMillSec(wait);

        for (u_int32_t i=0; i < _ledWrapper.getNumberLeds(); i=i+3)
        {
            _ledWrapper.setPixelColor(_settings->getStripHeight(), i+q, 0);        //turn every third pixel off
      }
    }
  }
  _ledWrapper.clearLeds();
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
               _ledWrapper.setPixelColor(_settings->getStripHeight(), i+q, Ws2811Wrapper::Wheel( (i+j) % 255));    //turn every third pixel on
            }
            _ledWrapper.show();

          Ws2811Wrapper::waitMillSec(wait);
          for (u_int32_t i=0; i < _ledWrapper.getNumberLeds(); i=i+3)
          {
            _ledWrapper.setPixelColor(_settings->getStripHeight(), i+q, 0);        //turn every third pixel off
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
    _ledWrapper.setPixelColor(_settings->getStripHeight(), i, _ledWrapper.Color(0, 0, 0, 0 ) );
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


      _ledWrapper.setPixelColor(_settings->getStripHeight(), counter, currentColor);
      _ledWrapper.show();
      if (_running == false) return;

       Ws2811Wrapper::waitMillSec(wait);
    }
    blink(10, 1000);
  }

    _ledWrapper.clearLeds();

}


void LightSystem::neoRand()
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
    for (u_int32_t i = 0; i < _ledWrapper.getNumberLeds(); i++)
    {

      if (_ledWrapper.getPixelColor(1, i) == _ledWrapper.Color(255,255,255))
          _ledWrapper.setPixelColor(_settings->getStripHeight(), i, saved); // return colour after sparkle


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

      _ledWrapper.setPixelColor(_settings->getStripHeight(), i, _ledWrapper.Color(_red, _green, _blue)); // set colour

    }  //  for(int i=0;i<NUMPIXELS;i++){

    cnt += 1;
    if (cnt >= _ledWrapper.getNumberLeds()) cnt = 0;


    tw = _cno - 0xDF;

    if (tw < _ledWrapper.getNumberLeds() && tw == 0 )
    {
      _ledWrapper.setPixelColor(_settings->getStripHeight(), tw, _ledWrapper.Color(255,255,255));
    }


    _ledWrapper.show();    // This sends the updated pixel colors to the string.
    if(_running == false)
        return;
    Ws2811Wrapper::waitMillSec(_wait);  // Delay (milliseconds).
  }

    _ledWrapper.clearLeds();

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
      _ledWrapper.setPixelColor(_settings->getStripHeight(), i, r1, g1, b1);
    }
    _ledWrapper.show();

    if(_running == false)
        return;
    //  Adjust the delay here, if you'd like.  Right now, it randomizes the
    //  color switch delay to give a sense of realism
    Ws2811Wrapper::waitMillSec((rand() % (150 - 20 + 1)) + 20);
  }
  _ledWrapper.clearLeds();
}

void LightSystem::chaser(u_int32_t wait)
{
    ws2811_return_t renderResults;

    _ledWrapper.clearLeds();
    for(int counter = 0; counter < 100; counter++)
    {
        for(u_int32_t counter = 0; counter < _ledWrapper.getNumberLeds(); counter++)
        {
            _ledWrapper.setPixelColor(_settings->getStripHeight(), counter, _red, _green , _blue);
            renderResults = _ledWrapper.show();
            if(renderResults != WS2811_SUCCESS)
            {
                _logger->logWarning(_ledWrapper.ws2811_get_return_t_str(renderResults));
                return;
            }
            if(_running == false)
                return;
            Ws2811Wrapper::waitMillSec(wait);

        }
    }
    _ledWrapper.clearLeds();
}



void LightSystem::cyclon(ws2811_led_t c, int width, int speed)
{
    for(int counter = 0; counter < 25; counter++)
    {
      // First slide the leds in one direction
      for(u_int32_t i = 0; i <= _ledWrapper.getNumberLeds() - width; i++)
      {
        for(int j=0; j<width; j++)
        {
            _ledWrapper.setPixelColor(_settings->getStripHeight(), i + j, c);
        }

        _ledWrapper.show();

        // now that we've shown the leds, reset to black for next loop
        for(int j=0; j<5; j++)
        {
            _ledWrapper.setPixelColor(_settings->getStripHeight(), i + j, _ledWrapper.Color(0,0,0));
          //leds[i+j] = CRGB::Black;
        }
        if(_running == false)
            return;
        Ws2811Wrapper::waitMillSec(speed);
        _ledWrapper.clearLeds();
      }

      // Now go in the other direction.
      for(u_int32_t i = _ledWrapper.getNumberLeds() - width; i == 0; i--)
      {
        for(int j=0; j<width; j++)
        {
        _ledWrapper.setPixelColor(_settings->getStripHeight(), i + j, c);
        }
        if(_running == false)
            return;
        _ledWrapper.show();
        for(int j=0; j<width; j++)
        {
           _ledWrapper.setPixelColor(_settings->getStripHeight(), i + j, _ledWrapper.Color(0,0,0));
        }
        if(_running == false)
            return;
        Ws2811Wrapper::waitMillSec(speed);
      }

      _ledWrapper.clearLeds();
    }
    _ledWrapper.clearLeds();
}


void LightSystem::colorWipe(ws2811_led_t color, u_int32_t waitms)
{
  _ledWrapper.clearLeds();
  for(int counter = 0; counter < 25; counter++)
  {

      for(uint32_t i=0; i < _ledWrapper.getNumberLeds(); i++)
      {
        _ledWrapper.setPixelColor(_settings->getStripHeight(), i, color);
        Ws2811Wrapper::waitMillSec(waitms);
        _ledWrapper.show();
        if(_running == false)
            return;
        Ws2811Wrapper::waitMillSec(waitms);

      }
 }
    _ledWrapper.clearLeds();
}

void LightSystem::halfnHalf(ws2811_led_t halfN, ws2811_led_t nHalf, u_int32_t delayMs)
{
    u_int32_t half = _ledWrapper.getNumberLeds() / 2;
    u_int32_t counter = 0;

    for(int count = 0; count < 25; count++)
    {
         _ledWrapper.clearLeds();
        for(counter = 0; counter < half; counter++)
        {
            _ledWrapper.setPixelColor(_settings->getStripHeight(), counter, halfN);
            _ledWrapper.show();

            Ws2811Wrapper::waitMillSec(delayMs);

        }
        if(_running == false)
            return;
        for(; counter < _ledWrapper.getNumberLeds(); counter++)
        {
            _ledWrapper.setPixelColor(_settings->getStripHeight(), counter, nHalf);
            _ledWrapper.show();
            Ws2811Wrapper::waitMillSec(delayMs);
        }
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
      _ledWrapper.setPixelColor(_settings->getStripHeight(), i,  Ws2811Wrapper::Wheel((i+j) & 255));
    }
    _ledWrapper.show();
    if(_running == false)
        return;
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
           _ledWrapper.setPixelColor(_settings->getStripHeight(), i, Ws2811Wrapper::Wheel(((i * 256 / _ledWrapper.getNumberLeds()) + j) & 255));

        _ledWrapper.show();
        if(_running == false)
            return;
        Ws2811Wrapper::waitMillSec(wait);
    }
    _ledWrapper.clearLeds();
}

void LightSystem::colorThirds(ws2811_led_t startColor, ws2811_led_t middleColor, ws2811_led_t finishColor, u_int32_t delay)
{
    ws2811_return_t renderResults = WS2811_SUCCESS;
    ws2811_led_t colors[] = {startColor, middleColor, finishColor};

    u_int32_t third = _ledWrapper.getNumberLeds() / 3;
    u_int32_t counter = 0;
    u_int32_t  currentDivision = third;
    u_int8_t colorNumber = 1;

    for(u_int32_t count = 0; count < 25; count++)
    {
        ws2811_led_t currentColor = startColor;

        if (  (renderResults = _ledWrapper.clearLeds() ) == WS2811_SUCCESS)
        {
            for(counter = 0; counter < _ledWrapper.getNumberLeds(); counter++)
            {
                if(_running == false)
                    return;

                if(counter >= currentDivision)
                {
                    ++colorNumber;
                    currentDivision = third * colorNumber;
                    currentColor = colors[colorNumber];
                }

                _ledWrapper.setPixelColor(_settings->getStripHeight(), counter, currentColor);
                if( (renderResults = _ledWrapper.show()) != WS2811_SUCCESS)
                {
                    _logger->logWarning("LightSystem::colorThirds Render Failed");
                    return;
                }

                Ws2811Wrapper::waitMillSec(delay);

            }

        }
    }
    _ledWrapper.clearLeds();
}

void LightSystem::colorForths(ws2811_led_t colorOne, ws2811_led_t colorTwo, ws2811_led_t colorThree,ws2811_led_t colorFour, u_int32_t delay)
{
    ws2811_return_t renderResults = WS2811_SUCCESS;
    ws2811_led_t colors[] = {colorOne, colorTwo, colorThree, colorFour};

    u_int32_t forths = _ledWrapper.getNumberLeds() / 4;
    u_int32_t counter = 0;
    u_int32_t  currentDivision = forths;
    u_int8_t colorNumber = 1;

    ws2811_led_t currentColor = colorOne;

    for(int count = 0; count < 25; count++)
    {
        _ledWrapper.clearLeds();

        for(counter = 0; counter < _ledWrapper.getNumberLeds(); counter++)
        {
            if(false == _running)
                return;

            if(counter >= currentDivision)
            {
                ++colorNumber;
                currentDivision = forths * colorNumber;
                currentColor = colors[colorNumber];
            }

            _ledWrapper.setPixelColor(_settings->getStripHeight(), counter, currentColor);
            if( (renderResults = _ledWrapper.show()) != WS2811_SUCCESS)
            {
                _logger->logWarning("LightSystem::colorForths Render Failed");
                return;
            }
            Ws2811Wrapper::waitMillSec(delay);

        }
    }
    _ledWrapper.clearLeds();

}

void LightSystem::triChaser(ws2811_led_t c1, ws2811_led_t c2, ws2811_led_t c3, u_int32_t delay)
{

    ws2811_return_t renderResults = WS2811_SUCCESS;
    ws2811_led_t curcolor = c1;

    for(int counter = 0; counter < 25; counter++)
    {
        _ledWrapper.clearLeds();

        for(uint32_t i=0; i < _ledWrapper.getNumberLeds(); i++)
        {
            if(i % 3 == 0)
                curcolor = c3;
            else if(i % 2 == 0)
                curcolor = c2;
            else
                curcolor = c1;

            _ledWrapper.setPixelColor(_settings->getStripHeight(), i, curcolor);

            if( (renderResults = _ledWrapper.show()) != WS2811_SUCCESS)
            {
                _logger->logWarning("triChaser - Render Failed");
                break;

            }

            if(_running == false)
                return;

            Ws2811Wrapper::waitMillSec(delay);

        }
    }
    _ledWrapper.clearLeds();
}
