#include "ilightshow.h"
#include <math.h>

#include <QDebug>
#include <random>



ILightShow::ILightShow(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString& showParms) :
        _ledWrapper(ledWrapper), _lightShow(lightShow), _showParms(showParms)
{
    _logger = DNRLogger::instance();
    _settings = SystemSettings::getInstance();

    srand(time(nullptr));
    _userId = 1;
    _running = false;
    _wait = 100;
    _clearOnStart = false;
    _clearOnFinish = false;
    _numMins = 1;
    _width = 5;
    _colorEvery = 2;
    _color1 = _ledWrapper->Color(125, 125, 125);
    _color2 = _ledWrapper->Color(0, 125, 125);
    _color3 = _ledWrapper->Color(125, 125, 0);
    _color4 = _ledWrapper->Color(125, 0, 125);

    QJsonObject jsonColors;
    QJsonObject jsonColor;

  
   QJsonDocument doc = QJsonDocument::fromJson(showParms.toUtf8());

   if(!doc.isNull())
   {
       if(doc.isObject())
       {

            _showParmsJson = doc.object();

            if(_showParmsJson.value("UserID").isString())
                _userId = _showParmsJson.value("UserID").toString().toInt();

            if(_showParmsJson.value("brightness").isString())
                _brightness = _showParmsJson.value("brightness").toString().toInt();

            if(_showParmsJson.value("delay").isString())
                _wait = _showParmsJson.value("delay").toString().toInt();

            _clearOnStart = (bool)_showParmsJson.value("clearStart").toInt();
            _clearOnFinish = (bool)_showParmsJson.value("clearFinish").toInt();
            _useGammaCorrection = (bool)_showParmsJson.value("gammaCorrection").toInt();

            if(_showParmsJson.value("minutes").isString())
                _numMins = _showParmsJson.value("minutes").toString().toDouble();

            if(_showParmsJson.value("width").isString())
                _width = _showParmsJson.value("width").toString().toInt();

            if(_showParmsJson.value("colorEvery").isString())
                _colorEvery = _showParmsJson.value("colorEvery").toString().toInt();

            if(_showParmsJson.value("matrixText").isString())
                _matrixText = _showParmsJson.value("matrixText").toString();

           if(_showParmsJson["colors"].isObject())
           {
               jsonColors = _showParmsJson["colors"].toObject();

               if(jsonColors["color4"].isObject())
               {
                   jsonColor = jsonColors["color4"].toObject();
                   _r = jsonColor.value("r").toInt();
                   _g = jsonColor.value("g").toInt();
                   _b = jsonColor.value("b").toInt();

                  _color4 = _ledWrapper->Color(_r, _g, _b);

               }

               if(jsonColors["color3"].isObject())
               {
                   jsonColor = jsonColors["color3"].toObject();
                   _r = jsonColor.value("r").toInt();
                   _g = jsonColor.value("g").toInt();
                   _b = jsonColor.value("b").toInt();
                  _color3 = _ledWrapper->Color(_r, _g, _b);

               }

               if(jsonColors["color2"].isObject())
               {
                   jsonColor = jsonColors["color2"].toObject();
                   _r = jsonColor.value("r").toInt();
                   _g = jsonColor.value("g").toInt();
                   _b = jsonColor.value("b").toInt();
                  _color2 = _ledWrapper->Color(_r, _g, _b);

               }

               if(jsonColors["color1"].isObject())
               {
                   jsonColor = jsonColors["color1"].toObject();
                   _r = jsonColor.value("r").toInt();
                   _g = jsonColor.value("g").toInt();
                   _b = jsonColor.value("b").toInt();
                  _color1 = _ledWrapper->Color(_r, _g, _b);

               }

           }


       }
       else
       {
           _logger->logWarning("ILightShow::ShowFire Invalid JSON");
       }
   }
   else
   {
       _logger->logWarning("ILightShow::ShowFire Document is not an object");
   }

}


void ILightShow::run()
{
    _running = true;
    if(_clearOnStart)
        _ledWrapper->clearLeds();

    _settings->setBrightness(_brightness);
     gammaCorrection();

    _endTime =  time(nullptr) + (_numMins * 60);
    
    startShow();

  _running = false;

    if(_clearOnFinish)
        _ledWrapper->clearLeds();

     emit showComplete(this);



}

void ILightShow::stopShow()
{
    _running = false;
    wait();
}

LedLightShows ILightShow::getLightShow() const
{
    return _lightShow;
}


void ILightShow::gammaCorrection()
{

   /* for(int  counter = 0; counter <= 255; counter++)
    {
        _gammaCorrection[counter] = (!_useGammaCorrection) ? counter :
                (int)(pow((float)counter / (float)255.00, _settings->getGamma()) * 255.00 + 0.5);
    }
*/
    if(true == _useGammaCorrection)
        _ledWrapper->setCustomGammaCorrection(_settings->getGamma());
      else
        _ledWrapper->setCustomGammaCorrection(0);

}


QString ILightShow::getShowName()
{
    const char* lightShowNames[] = {LIGHT_SHOWS(LIGHT_SHOWS_SHOW)};
    return lightShowNames[_lightShow];
}

QString ILightShow::getShowParms() const
{
    return _showParms;
}

bool ILightShow::isRunning() const
{
    return _running;
}


int ILightShow::genRand(unsigned int min, unsigned int max)
{
    std::random_device seeder;
    std::mt19937 engine(seeder());
    std::uniform_int_distribution<unsigned int> dist(min, max);
    return dist(engine);

}

int ILightShow::getUserId() const
{
    return _userId;
}


ws2811_led_t ILightShow::gamaColor(ws2811_led_t inColor)
{
    return (ws2811_led_t) ((inColor / 255) ^ (ws2811_led_t)(1 / _settings->getGamma())) * 255;
}

void ILightShow::drawCircle(int xc, int yc, int x, int y)
{


    _ledWrapper->setPixelColor(xc+x, yc+y, _color1);
    _ledWrapper->setPixelColor(xc-x, yc+y, _color1);
    _ledWrapper->setPixelColor(xc+x, yc-y, _color1);
    _ledWrapper->setPixelColor(xc-x, yc-y, _color1);
    _ledWrapper->setPixelColor(xc+y, yc+x, _color1);
    _ledWrapper->setPixelColor(xc-y, yc+x, _color1);
    _ledWrapper->setPixelColor(xc+y, yc-x, _color1);
    _ledWrapper->setPixelColor(xc-y, yc-x, _color1);

}

void ILightShow::circleBres(int xc, int yc, int r)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;
    drawCircle(xc, yc, x, y);
    while (y >= x)
    {
        // for each pixel we will
        // draw all eight pixels

        x++;

        // check for decision parameter
        // and correspondingly
        // update d, x, y
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;
        drawCircle(xc, yc, x, y);

    }
}






//encoded = ((original / 255) ^ (1 / gamma)) * 255
//Gamma decoding is used to restore the original values, so the formula for that is:
//original = ((encoded / 255) ^ gamma) * 255



/*
 *
 *
 * def bouncing_balls(strip, playtime, ball_count=2, wait_ms=200):
    """Shows an accelerated pixel with physicslike a ball in a flipper game"""
    import time, math
    start_time = time.time()
    ClockTimeSinceLastBounce = [0 for i in range(ball_count)]
    StartHeight=1

    for i in range(ball_count):
        ClockTimeSinceLastBounce[i] = time.time()

    Height = [0 for i in range(ball_count)]
    Position = [0 for i in range(ball_count)]
    ImpactVelocity = [0 for i in range(ball_count)]
    ImpactVelocityStart= math.sqrt(-2 * -9.81 * 1)
    Dampening = [0 for i in range(ball_count)]
    TimeSinceLastBounce = [0 for i in range(ball_count)]

    for i in range(0,ball_count,1):
        last_ClockTimeSinceLastBounce = ClockTimeSinceLastBounce[i]
        ClockTimeSinceLastBounce[i] = time.time() - last_ClockTimeSinceLastBounce

        Height[i] = StartHeight
        Position[i] = 0
        ImpactVelocity[i] = math.sqrt(-2 * -9.81 * 1)
        TimeSinceLastBounce[i] = 0
        Dampening[i] = 0.90 - (float(i)/(ball_count**2))
    act_time = time.time()
    while ((act_time+ playtime)> time.time()):
        for i in range(ball_count):
            TimeSinceLastBounce[i] = time.time() - ClockTimeSinceLastBounce[i]
            Height[i] = 0.5 * (-9.81) * (TimeSinceLastBounce[i]**2) + ImpactVelocity[i] * TimeSinceLastBounce[i]
            if (Height[i] < 0):
                Height[i] = 0
                ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i]
                ClockTimeSinceLastBounce[i] = time.time()
                if (ImpactVelocity[i] < 0.01):
                    ImpactVelocity[i] = ImpactVelocityStart

            Position[i] = round(Height[i] * (strip.numPixels()-1)/StartHeight)
        for i in range(ball_count):
            strip.setPixelColorRGB(Position[i], 0, 0,255)
        strip.show()
        for i in range(strip.numPixels()):
            strip.setPixelColorRGB(i, 0,0,0)
 *
 * */
