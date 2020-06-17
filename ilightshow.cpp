#include "ilightshow.h"
#include <QDebug>


ILightShow::ILightShow(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString& showParms) :
        _ledWrapper(ledWrapper), _lightShow(lightShow), _showParms(showParms)
{
    _logger = DNRLogger::instance();
    _settings = SystemSettings::getInstance();

    srand(time(nullptr));
    _running = false;
    _colorEvery = 0;

    QJsonObject jsonObject;
    QJsonObject jsonColors;
    QJsonObject jsonColor;

  
   QJsonDocument doc = QJsonDocument::fromJson(showParms.toUtf8());

   if(!doc.isNull())
   {
       if(doc.isObject())
       {

           jsonObject = doc.object();
           _ledWrapper->setBrightness(jsonObject.value("brightness").toString().toInt());
           _wait = jsonObject.value("delay").toString().toInt();
           _clearOnStart = (bool)jsonObject.value("clearStart").toInt();
           _clearOnFinish = (bool)jsonObject.value("clearFinish").toInt();
           _numLoops = jsonObject.value("numLoops").toString().toInt();
           _width = jsonObject.value("width").toString().toInt();
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
           return;
       }
   }
   else
   {
       _logger->logWarning("ShowFire::ShowFire Document is not an object");
       return;

   }

}


void ILightShow::run()
{
    _running = true;
    if(_clearOnStart)
        _ledWrapper->clearLeds();

    _settings->setBrightness(_brightness);

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

