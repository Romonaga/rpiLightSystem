#include "ilightshow.h"
#include <QDebug>


ILightShow::ILightShow(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString& showParms) :
        _ledWrapper(ledWrapper), _lightShow(lightShow), _showParms(showParms)
{
    _logger = DNRLogger::instance();
    _settings = SystemSettings::getInstance();

    _running = false;

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
           _clearOnStart = jsonObject.value("clearStart").toString().toInt();
           _clearOnFinish = jsonObject.value("clearFinish").toString().toInt();
           _numLoops = jsonObject.value("numLoops").toString().toInt();
           _width = jsonObject.value("width").toString().toInt();

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

    startShow();
    _running = false;
    emit showComplete(this);
}

void ILightShow::stopShow()
{
    _running = false;
    wait();
    if(_clearOnFinish)
        _ledWrapper->clearLeds();

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
