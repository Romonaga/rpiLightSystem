#include "ilightshow.h"
#include <QDebug>


ILightShow::ILightShow(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString& showParms) :
        _ledWrapper(ledWrapper), _lightShow(lightShow), _showParms(showParms)
{
    _logger = DNRLogger::instance();
    _settings = SystemSettings::getInstance();

    srand(time(nullptr));

    _running = false;
    _wait = 0;
    _clearOnStart = false;
    _clearOnFinish = false;
    _numMins = 1;
    _width = 1;
    _colorEvery = 2;
    _color1 = Ws2811Wrapper::Color(125, 125, 125);
    _color2 = Ws2811Wrapper::Color(0, 125, 125);
    _color3 = Ws2811Wrapper::Color(125, 0, 125);
    _color4 = Ws2811Wrapper::Color(125, 125, 0);

    QJsonObject jsonObject;
    QJsonObject jsonColors;
    QJsonObject jsonColor;

  
   QJsonDocument doc = QJsonDocument::fromJson(showParms.toUtf8());

   if(!doc.isNull())
   {
       if(doc.isObject())
       {

            jsonObject = doc.object();
            if(jsonObject.value("brightness").isString())
                _ledWrapper->setBrightness(jsonObject.value("brightness").toString().toInt());

            if(jsonObject.value("delay").isString())
                _wait = jsonObject.value("delay").toString().toInt();

            _clearOnStart = (bool)jsonObject.value("clearStart").toInt();
            _clearOnFinish = (bool)jsonObject.value("clearFinish").toInt();

            if(jsonObject.value("minutes").isString())
                _numMins = jsonObject.value("minutes").toString().toDouble();

            if(jsonObject.value("width").isString())
                _width = jsonObject.value("width").toString().toInt();

            if(jsonObject.value("colorEvery").isString())
                _colorEvery = jsonObject.value("colorEvery").toString().toInt();

           if(jsonObject["colors"].isObject())
           {
               jsonColors = jsonObject["colors"].toObject();

               if(jsonColors["color4"].isObject())
               {
                   jsonColor = jsonColors["color4"].toObject();
                   _r = jsonColor.value("r").toInt();
                   _g = jsonColor.value("g").toInt();
                   _b = jsonColor.value("b").toInt();
                  _color4 = Ws2811Wrapper::Color(_r, _g, _b);

               }

               if(jsonColors["color3"].isObject())
               {
                   jsonColor = jsonColors["color3"].toObject();
                   _r = jsonColor.value("r").toInt();
                   _g = jsonColor.value("g").toInt();
                   _b = jsonColor.value("b").toInt();
                  _color3 = Ws2811Wrapper::Color(_r, _g, _b);

               }

               if(jsonColors["color2"].isObject())
               {
                   jsonColor = jsonColors["color2"].toObject();
                   _r = jsonColor.value("r").toInt();
                   _g = jsonColor.value("g").toInt();
                   _b = jsonColor.value("b").toInt();
                  _color2 = Ws2811Wrapper::Color(_r, _g, _b);

               }

               if(jsonColors["color1"].isObject())
               {
                   jsonColor = jsonColors["color1"].toObject();
                   _r = jsonColor.value("r").toInt();
                   _g = jsonColor.value("g").toInt();
                   _b = jsonColor.value("b").toInt();
                  _color1 = Ws2811Wrapper::Color(_r, _g, _b);


               }

           }


       }
       else
       {
           _logger->logWarning("ShowFire::ShowFire Invalid JSON");
       }
   }
   else
   {
       _logger->logWarning("ShowFire::ShowFire Document is not an object");
   }

}


void ILightShow::run()
{
    _running = true;
    if(_clearOnStart)
        _ledWrapper->clearLeds();

    _settings->setBrightness(_brightness);

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



QString ILightShow::getShowName()
{
    const char* lightShowNames[] = {LIGHT_SHOWS(LIGHT_SHOWS_STRING)};
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

int ILightShow::genRand(int min, int max)
{
   return (rand() % max) + min;
}

