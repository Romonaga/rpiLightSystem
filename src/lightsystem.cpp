#include <unistd.h>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

//LightShow system
#include "lightsystem.h"
#include "playlistmanager.h"


//Shows
#include "showblink.h"
#include "showflame.h"
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
#include "showcolor.h"
#include "showcolorevery.h"
#include "showtwinkle.h"
#include "showPulse.h"
#include "showscanner.h"



LightSystem::LightSystem(QObject *parent) : QObject(parent)
{
    _settings = SystemSettings::getInstance();

    _started = false;
    _running = false;
    _mqq = nullptr;
    _logger = DNRLogger::instance();
    _logger->setDebugOut(_settings->getDbgLog());
    _motionFeature = nullptr;
    _lightSensorFeature = nullptr;
    _timeFeature = nullptr;
    _luxFeature = nullptr;

}


LightSystem::~LightSystem()
{

    if(_mqq != nullptr)
    {
        stopSystem();
    }

    if(_motionFeature)
    {
        _motionFeature->stop();
        delete _motionFeature;
    }

    if(_lightSensorFeature)
    {
        _lightSensorFeature->stop();
        delete _lightSensorFeature;
    }

    if(_timeFeature)
    {
        _timeFeature->stop();
        delete _timeFeature;
    }

    if(_luxFeature)
    {
        _luxFeature->stop();
        delete _luxFeature;
    }

    _ledWrapper.clearLeds();

    _logger->logInfo("~LightSystem Offline");
}

const char *LightSystem::getEnumName(int index)
{
    const char* _lightShowNames[] = {LIGHT_SHOWS(LIGHT_SHOWS_STRING)};
    return _lightShowNames[index];
}

void LightSystem::startShows()
{

    if(_running) return;

    _running = true;
    runShow();

    _logger->logInfo("LightSystem::startShows Started");

}


void LightSystem::stopShows()
{
    if(false == _running) return;

   _running = false;

    if(_runningShows.count() > 0)
        _runningShows[0]->stopShow();

   _ledWrapper.clearLeds();
   _logger->logInfo("LightSystem::stopShows Stopped");
}

void LightSystem::saveuserPlayList(QJsonObject jsonObject)
{
    std::stringstream info;

    info << "saveuserPlayList " << jsonObject.value("playlistName").toString().toStdString().c_str();
    _logger->logInfo(info.str());

    PlayListManager pmanager;
    if(_runningShows.count() != 0)
        pmanager.savePlayList(jsonObject.value("playlistName").toString(), jsonObject.value("UserID").toString().toInt(), _runningShows);
}

void LightSystem::deleteuserPlayList(QJsonObject jsonObject)
{
    std::stringstream info;

    info << "deleteuserPlayList " << jsonObject.value("playlistName").toString().toStdString().c_str();;
    _logger->logInfo(info.str());
    PlayListManager pmanager;
    pmanager.deletePlayList(jsonObject.value("UserID").toString().toInt(), jsonObject.value("playlistName").toString().toInt());
}


void LightSystem::playPlayList(QString playList)
{

    QJsonDocument doc = QJsonDocument::fromJson(playList.toUtf8());

    if(!doc.isNull())
    {
        if(doc.isArray())
        {

            QJsonArray jsonArray = doc.array();
            foreach (QJsonValue info, jsonArray)
            {
                QString msg = QJsonDocument::fromVariant(info.toVariant()).toJson(QJsonDocument::Compact);
                processMsgReceived(msg);
            }

        }
    }
}

void LightSystem::playuserPlayList(QJsonObject jsonObject)
{
    std::stringstream info;

    info << "playuserPlayList " << jsonObject.value("playlistName").toString().toStdString().c_str();;
    _logger->logInfo(info.str());

    PlayListManager pmanager;
    QString playList = pmanager.getPlayList(jsonObject.value("UserID").toString().toInt(),jsonObject.value("playlistName").toString().toInt());
    playPlayList(playList);

}

void LightSystem::processShow(QString msg, QJsonObject jsonObject)
{

    if(jsonObject.value("show").isString())
    {
        if(jsonObject.value("powerOn").isString())
        {
            startShows();
        }
        queueShow(static_cast<LedLightShows>(jsonObject.value("show").toString().toInt()),msg);
    }

}


void LightSystem::processPower(QJsonObject jsonObject)
{

    std::stringstream info;

    QString state = jsonObject.value("state").toString();
    info << "LightSystem::processPower state(" << state.toStdString().c_str() << ")";
    _logger->logInfo(info.str());

    if(state == "ON")
        startShows();
    else if(state == "OFF")
        stopShows();
}

void LightSystem::clearQueue()
{
    if(_runningShows.count() == 0) return;

    std::stringstream info;

    info << "LightSystem::clearQueue(" << _runningShows.count() << ")";
    _logger->logInfo(info.str());

    QList<ILightShow*> shows = _runningShows.toList();
    foreach (ILightShow* show, shows)
    {
        if(show->isRunning() == false)
        {
            cleanUpShow(show);
        }
        else
        {
            show->stopShow();
        }

    }
}

void LightSystem::chgBrightness(QJsonObject jsonObject)
{
    if(jsonObject.value("chgBrightness").isString())
    {

        _ledWrapper.setBrightness(jsonObject.value("chgBrightness").toString().toInt());
        if(_runningShows.count() == 0)
            _ledWrapper.show();

    }

}


void LightSystem::processMsgReceived(QString msg)
{
   std::stringstream info;

   info << "LightSystem::processMsgReceived: " << msg.toStdString().c_str();
   _logger->logInfo(info.str());

    QJsonObject jsonObject;

    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());

    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            jsonObject = doc.object();
            if(jsonObject.value("state").isString())
            {
                processPower(jsonObject);
            }
            else if(jsonObject.value("chgBrightness").isString())
            {
                chgBrightness(jsonObject);
            }
            else if(jsonObject.value("savePlaylist").toInt())
            {
                saveuserPlayList(jsonObject);
            }
            else if(jsonObject.value("playPlaylist").toInt())
            {
                playuserPlayList(jsonObject);
            }
            else if(jsonObject.value("deletePlaylist").toInt())
            {
                deleteuserPlayList(jsonObject);
            }
            else if(jsonObject.value("clearQueue").toInt())
            {
                clearQueue();
            }
            else
            {
                processShow(msg, jsonObject);
            }

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



void LightSystem::queueShow(const LedLightShows& show, const QString& showParms)
{
    std::stringstream info;


    _runningShowsMutex.lock();
    bool kickOff = (_runningShows.count() == 0) ? true : false;
    switch(show)
    {
        case Blink:
            _runningShows.append(new ShowBlink(&_ledWrapper, show, showParms));
            break;

        case Chaser:
            _runningShows.append(new ShowChaser(&_ledWrapper, show, showParms));
            break;

        case TC:
            _runningShows.append(new ShowTheaterChaser(&_ledWrapper, show, showParms));
            break;

        case TCR:
            _runningShows.append(new ShowTheaterChaseRainBow(&_ledWrapper, show, showParms));
            break;

        case Color3R:
            _runningShows.append(new ShowColor3R(&_ledWrapper, show, showParms));
            break;

        case Cyclon:
            _runningShows.append(new ShowCyclon(&_ledWrapper, show, showParms));
            break;

        case ColorWipe:
            _runningShows.append(new ShowColorWipe(&_ledWrapper, show, showParms));
            break;

        case HAndH:
            _runningShows.append(new ShowHnH(&_ledWrapper, show, showParms));
            break;

        case Rainbow:
            _runningShows.append(new ShowRainbow(&_ledWrapper, show, showParms));
            break;

        case RainbowCycle:
            _runningShows.append(new ShowRainbowCycle(&_ledWrapper, show, showParms));
            break;

        case NeoRand:
            _runningShows.append(new ShowNeoRand(&_ledWrapper, show, showParms));
            break;

        case Flame:
            _runningShows.append(new ShowFlame(&_ledWrapper, show, showParms));
            break;

        case ColorThirds:
            _runningShows.append(new ShowColor3(&_ledWrapper, show, showParms));
            break;

        case ColorForths:
            _runningShows.append(new ShowColor4(&_ledWrapper, show, showParms));
            break;

        case TriChaser:
            _runningShows.append(new ShowTriChaser(&_ledWrapper, show, showParms));
            break;

        case DisplayColor:
            _runningShows.append(new ShowColor(&_ledWrapper, show, showParms));
            break;

        case ColorEvery:
            _runningShows.append(new ShowColorEvery(&_ledWrapper, show, showParms));
            break;

        case TwinkleOverlay:
            _runningShows.append(new ShowTwinkle(&_ledWrapper, show, showParms));
            break;

        case PulseOverlay:
            _runningShows.append(new ShowPulse(&_ledWrapper, show, showParms));
            break;

        case Scanner:
            _runningShows.append(new ShowScanner(&_ledWrapper, show, showParms));
            break;


    default:
        _logger->logWarning("Unknown Show");

    }

    info << "LightSystem::queueShow(" << getEnumName(show) << ") Queue(" << _runningShows.count() << ")";
    _logger->logInfo(info.str());


    _runningShowsMutex.unlock();

    if(kickOff) runShow();
}

void LightSystem::logShow(ILightShow* show)
{
    if(false == _settings->getLogShows()) return;

    QSqlDatabase database = QSqlDatabase().addDatabase("QMYSQL","logShow");
    database.setHostName(_settings->getDBServer());
    database.setUserName(_settings->getDBUser());
    database.setPassword(_settings->getDBPwd());
    database.setDatabaseName(_settings->getDataBase());

    if(database.open())
    {
        std::stringstream sql;


        sql << "insert into showsRan(userID, systemID, showId, showParms) values(" <<
                show->getUserId() << "," <<
                _settings->getSystemId() << "," <<
                static_cast<int>(show->getLightShow()) << ",'" <<
                show->getShowParms().toStdString().c_str() << "')" ;

        QSqlQuery result(sql.str().c_str(), database);
        if(result.lastError().type() != QSqlError::NoError)
        {
            _logger->logInfo(result.lastError().text().toStdString());

        }

        database.close();


    }
    else
    {
        _logger->logInfo(database.lastError().text().toStdString());

    }

    QSqlDatabase::removeDatabase("logShow");

}


void LightSystem::runShow()
{
    if(false == _running) return;

    std::stringstream info;

    if(_runningShows.count() > 0)
    {

        info.str("");
        info << "LightSystem::runShow(" << _runningShows[0]->getShowName().toStdString().c_str() << ")";
        _logger->logInfo(info.str());

        connect(_runningShows[0], SIGNAL(showComplete(ILightShow*)), this, SLOT(showComplete(ILightShow*)));
        _runningShows[0]->start();

    }
}


void LightSystem::cleanUpShow(ILightShow* show)
{

    std::stringstream info;

    info << "LightSystem::cleanUpShow Show(" <<  show->getShowName().toStdString().c_str() << ")";
    _logger->logInfo(info.str());
    _runningShowsMutex.lock();
    if(show != nullptr) delete show;

    _runningShows.removeOne(show);
    _runningShowsMutex.unlock();

}


void LightSystem::showComplete(ILightShow* show)
{
    std::stringstream info;

    info << "LightSystem::showComplete Show(" <<  show->getShowName().toStdString().c_str() << ")";
    _logger->logInfo(info.str());
    show->stopShow();
    logShow(show);
    cleanUpShow(show);
    runShow();

}


void LightSystem::loadFeatures()
{
    _logger->logInfo("loadFeatures");

    QSqlDatabase database = QSqlDatabase().addDatabase("QMYSQL","rpiLightFeatures");

    database.setHostName(_settings->getInstance()->getDBServer());
    database.setUserName(_settings->getInstance()->getDBUser());
    database.setPassword(_settings->getInstance()->getDBPwd());
    database.setDatabaseName(_settings->getInstance()->getDataBase());
    if(database.open())
    {
        QString sql("select * from lightSystemFeatures where lightSystemId = ");
        sql.append(QString::number(_settings->getInstance()->getSystemId()));
        _logger->logInfo(sql.toStdString());
        QSqlQuery qry = database.exec(sql);

        if(qry.lastError().type() == QSqlError::NoError)
        {

            while(qry.next())
            {

                int lightfeatureId = qry.value("featureId").toInt();
                switch(lightfeatureId)
                {
                    case 1:
                    {
                        _logger->logInfo("Starting MotionLights Feature");
                        _motionFeature = new MotionLightsFeature(qry);
                        connect(_motionFeature, SIGNAL(motionLightsStateChange(MotionLightsFeature*, int)), this, SLOT(motionStateChange(MotionLightsFeature*, int)));
                        _motionFeature->start();
                    }
                    break;

                    case 2:
                    {
                        _logger->logInfo("Starting LightSensor Feature");
                        _lightSensorFeature = new LightSensorFeature(qry);
                        connect(_lightSensorFeature, SIGNAL(lightSensorStateChange(LightSensorFeature*, int)), this, SLOT(lightStateChange(LightSensorFeature*, int)));
                        _lightSensorFeature->start();
                    }
                    break;

                    case 3:
                    {
                        _logger->logInfo("Starting Time Feature");
                        _timeFeature = new TimeFeature(qry);
                        connect(_timeFeature, SIGNAL(timeStateChange(TimeFeature*, int)), this, SLOT(timeStateChange(TimeFeature*, int)));
                        _timeFeature->start();
                    }
                    break;

                    case 4:
                    {
                        _logger->logInfo("Starting LightLux Feature");
                        _luxFeature = new LightLuxFeature(qry);
                        connect(_luxFeature, SIGNAL(lightLuxStateChange(LightLuxFeature*, quint32)), this, SLOT(lightLuxStateChange(LightLuxFeature*, quint32)));
                        _luxFeature->start();
                    }
                    break;

                }

            }


        }
        else
        {
            fprintf(stderr, "%s", qry.lastError().text().toStdString().c_str());
        }
        database.close();
    }
    else
    {
        fprintf(stderr, "%s", database.lastError().text().toStdString().c_str());
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
       renderResults = _ledWrapper.initStrip(_settings->getStripWidth(), _settings->getStripHeight(), (LedStripType)_settings->getStripType(), _settings->getDma(), _settings->getGpio(), false);
       if(renderResults != WS2811_SUCCESS)
       {
           info.str("");
           info << "LightSystem start() Failed Code(" << renderResults << ") Msg(" <<
                   _ledWrapper.ws2811_get_return_t_str(renderResults) << ")";
           _logger->logWarning(info.str());
           _started = false;
           return _started;
       }

        _logger->logInfo("Setting Up MQTT");
        _mqq = new MqttReceiver(_settings->getMqttBroker(), _settings->getHostName(), 1);
        connect(_mqq, SIGNAL(msgReceived(QString)), this, SLOT(processMsgReceived(QString)));


        _logger->logInfo("Configuring LED Strip");
       _ledWrapper.setBrightness(_settings->getBrightness());
       _ledWrapper.setClearOnExit(true);

      loadFeatures();

    }
    else
    {
        _logger->logWarning("Problem Loadng Config File.");
    }
    return _started;
}

void LightSystem::stopSystem()
{
    if(false == _started) return;
    _logger->logInfo("LightSystem::stopSystem Stopping");
    _started = false;
    stopShows();
    _mqq->stop();
    _logger->logInfo("LightSystem::stopSystem Stopped");
}


void LightSystem::motionStateChange(MotionLightsFeature* feature, int state)
{
    std::stringstream info;

    info << "LightSystem::motionStateChange: " << state;
    _logger->logInfo(info.str());

    if(state == 1 && _runningShows.count() == 0)
    {
        info.str("");
        info << "motionStateChange Run PlayList(" << feature->getFeaturePlayList() << ")";
        _logger->logInfo(info.str());

        PlayListManager pmanager;
        QString playList = pmanager.getPlayList(feature->getFeaturePlayList());
        playPlayList(playList);
        startShows();
    }
    else if(state == 0)
    {
        stopShows();
        _ledWrapper.clearLeds();
    }

    _logger->logInfo(info.str());
}

void LightSystem::lightStateChange(LightSensorFeature *feature, int state)
{
    std::stringstream info;

    info << "LightSystem::lightStateChange: " << state;
    _logger->logInfo(info.str());

    if(state == 0 && _runningShows.count() == 0)
    {
        info.str("");
        info << "lightStateChange Run PlayList(" << feature->getFeaturePlayList() << ")";
        _logger->logInfo(info.str());

        PlayListManager pmanager;
        QString playList = pmanager.getPlayList(feature->getFeaturePlayList());
        playPlayList(playList);
        startShows();
    }
    else if(state == 1)
    {
        info.str("");
       info << "lightStateChange Stop Lights";
       _logger->logInfo(info.str());
        stopShows();
        _ledWrapper.clearLeds();
    }

}


void LightSystem::timeStateChange(TimeFeature *feature, int state)
{
    std::stringstream info;

    info << "LightSystem::timeStateChange: " << state;
    _logger->logInfo(info.str());

    if(state == 1 && _runningShows.count() == 0)
    {
        info.str("");
        info << "timeStateChange Run PlayList(" << feature->getFeaturePlayList() << ")";
        _logger->logInfo(info.str());

        PlayListManager pmanager;
        QString playList = pmanager.getPlayList(feature->getFeaturePlayList());
        playPlayList(playList);
        startShows();
    }
    else if(state == 0)
    {
        info.str("");
       info << "timeStateChange Stop Lights";
       _logger->logInfo(info.str());
        stopShows();
        _ledWrapper.clearLeds();
    }
}


void LightSystem::lightLuxStateChange(LightLuxFeature *feature, quint32 lux)
{
    std::stringstream info;

    info << "LightSystem::lightLuxStateChange: " << lux;
    _logger->logInfo(info.str());


    if(_runningShows.count() == 0)
    {
        if(feature->getLuxThreshHold() <= lux)
        {
            info.str("");
            info << "lightLuxStateChange Run PlayList(" << feature->getFeaturePlayList() << ")";
            _logger->logInfo(info.str());

            PlayListManager pmanager;
            QString playList = pmanager.getPlayList(feature->getFeaturePlayList());
            playPlayList(playList);
            startShows();
        }
    }


    int  brightness = 255 - lux;
    brightness = (brightness <= 0) ? 1 : brightness;

    _ledWrapper.setBrightness(brightness);

}
