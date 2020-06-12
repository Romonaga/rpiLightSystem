#include "lightsystem.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

//Shows
#include "showblink.h"
#include "showfire.h"
#include "showchaser.h"
#include "showtheaterchaser.h"
#include "showtheaterchaserainbow.h"
#include "showcolor3r.h"
#include "showcyclon.h"
#include "showcolorwipe.h"
#include "showhnh.h"
#include "showrainbow.h"
#include "showrainbowcycle.h"
#include "showneorand.h"
#include "showcolor3.h"
#include "showcolor4.h"
#include "showtrichaser.h"





LightSystem::LightSystem(QObject *parent) : QObject(parent)
{
    _settings = SystemSettings::getInstance();

    _started = false;
    _running = false;
    _mqq = nullptr;
    _logger = DNRLogger::instance();
    _logger->setDebugOut(_settings->getDbgLog());

}


LightSystem::~LightSystem()
{
    _logger->logInfo("~LightSystem Going Offline");
    if(_mqq != nullptr)
    {
        stopSystem();


    }
    _ledWrapper.clearLeds();
}

const char *LightSystem::getEnumName(int index)
{
    const char* _lightShowNames[] = {LIGHT_SHOWS(LIGHT_SHOWS_STRING)};
    return _lightShowNames[index];
}

void LightSystem::startShows()
{

    if(_running) return;

    _logger->logInfo("LightSystem::startShows Starting");
    _running = true;
    runShow();
    _logger->logInfo("LightSystem::startShows Started");
}


void LightSystem::stopShows()
{
    if(false == _running) return;

    _logger->logInfo("LightSystem::stopShows Stopping Lights");
   _running = false;

    if(_runningShows.count() > 0)
        _runningShows[0]->stopShow();

   _ledWrapper.clearLeds();
   _logger->logInfo("LightSystem::stopShows Lights Stopped");
}

void LightSystem::processMsgReceived(QString msg)
{
   //fprintf(stderr," LightSystem::processMsgReceived: %s\r\n", msg.toStdString().c_str());

    QJsonObject jsonObject;
    QString state;

    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());

    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            jsonObject = doc.object();
            if(jsonObject.value("state").isString())
            {
                _logger->logInfo("State Command Received");
                state = jsonObject.value("state").toString();

                if(state == "ON")
                    startShows();
                else
                    stopShows();
            }

            _settings->setBrightness(jsonObject.value("brightness").toString().toInt());
            _ledWrapper.setBrightness(_settings->getBrightness());
            QJsonArray jsonArray = jsonObject["shows"].toArray();

            foreach (const QJsonValue & value, jsonArray)
                queueShow(static_cast<LedLightShows>(value.toString().toInt()));



        }
        else
        {
            _logger->logWarning("LightSystem::processMsgReceived Invalid JSON");
            return;
        }
    }
    else
    {
        _logger->logWarning("LightSystem::processMsgReceived Document is not an object");
        return;

    }


}



void LightSystem::queueShow(const LedLightShows& show)
{
    std::stringstream info;
    info << "LightSystem::queueShow Show(" << getEnumName(show) << ")";
    _logger->logInfo(info.str());


    _runningShowsMutex.lock();
    bool kickOff = (_runningShows.count() == 0) ? true : false;
    switch(show)
    {
        case Blink:
            _runningShows.append(new ShowBlink(_settings, &_ledWrapper, show, ""));
            break;

        case Chaser:
            _runningShows.append(new ShowChaser(_settings, &_ledWrapper, show, ""));
            break;

        case TC:
            _runningShows.append(new ShowTheaterChaser(_settings, &_ledWrapper, show, ""));
            break;

        case TCR:
            _runningShows.append(new ShowTheaterChaseRainBow(_settings, &_ledWrapper, show, ""));
            break;

        case Color3R:
            _runningShows.append(new ShowColor3R(_settings, &_ledWrapper, show, ""));
            break;

        case Cyclon:
            _runningShows.append(new ShowCyclon(_settings, &_ledWrapper, show, ""));
            break;

        case ColorWipe:
            _runningShows.append(new ShowColorWipe(_settings, &_ledWrapper, show, ""));
            break;

        case HAndH:
            _runningShows.append(new ShowHnH(_settings, &_ledWrapper, show, ""));
            break;

        case Rainbow:
            _runningShows.append(new ShowRainbow(_settings, &_ledWrapper, show, ""));
            break;

        case RainbowCycle:
            _runningShows.append(new ShowRainbowCycle(_settings, &_ledWrapper, show, ""));
            break;

        case NeoRand:
            _runningShows.append(new ShowNeoRand(_settings, &_ledWrapper, show, ""));
            break;

        case Flame:
            _runningShows.append(new ShowFire(_settings, &_ledWrapper, show, ""));
            break;

        case ColorThirds:
            _runningShows.append(new ShowColor3(_settings, &_ledWrapper, show, ""));
            break;

        case ColorForths:
            _runningShows.append(new ShowColor4(_settings, &_ledWrapper, show, ""));
            break;

        case TriChaser:
            _runningShows.append(new ShowTriChaser(_settings, &_ledWrapper, show, ""));
            break;

    default:
        _logger->logWarning("Unknown Show");

    }
    _runningShowsMutex.unlock();

    if(kickOff) runShow();
}

void LightSystem::runShow()
{
    if(false == _running) return;

    std::stringstream info;

    if(_runningShows.count() > 0)
    {

        info.str("");
        info << "LightSystem::runShow Show(" << _runningShows[0]->getShowName().toStdString().c_str() << ")";
        _logger->logInfo(info.str());
        connect(_runningShows[0], SIGNAL(showComplete(ILightShow*)), this, SLOT(showComplete(ILightShow*)));
        _runningShows[0]->start();
    }
}


void LightSystem::showComplete(ILightShow* show)
{

    std::stringstream info;

    info << "LightSystem::showComplete Show(" <<  show->getShowName().toStdString().c_str() << ")";
    _logger->logInfo(info.str());
    show->stopShow();
    _runningShowsMutex.lock();
    if(show != nullptr) delete show;

    _runningShows.removeFirst();
    _runningShowsMutex.unlock();
    runShow();
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
    _logger->logInfo("LightSystem::stopSystem Stopping");
    _started = false;
    stopShows();
    _mqq->stop();
    _logger->logInfo("LightSystem::stopSystem Stopped");
}


