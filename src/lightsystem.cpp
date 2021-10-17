#include <unistd.h>
#include <linux/kernel.h>       /* for struct sysinfo */
#include <sys/sysinfo.h>
#include <iomanip>
#include <stdio.h>

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
#include "showpulse.h"
#include "showscanner.h"
#include "showfade.h"
#include "showbouncingballs.h"
#include "matrixart.h"
#include "matrixscrolltext.h"
#include "matrixtrip.h"
#include "showdelay.h"
#include "matrixcircle.h"
#include "matrixsquare.h"
#include "matrixtriangle.h"
#include "matrixcreatejpg.h"
#include "matrixdisplayfile.h"
#include "matrixvolbars.h"
#include "matrixscrolltime.h"
#include "matrixantart.h"
#include "matrixkaleidoscope.h"
#include "matrixcolorevolution.h"
#include "matrixgameoflife.h"
#include "matrixmazecreator.h"


LightSystem::LightSystem(QObject *parent) : QObject(parent)
{
    _logger = DNRLogger::instance();

    _settings = SystemSettings::getInstance();

    _started = false;
    _running = false;
    _mqq = nullptr;
    _twitch = nullptr;

    _motionFeature = nullptr;
    _lightSensorFeature = nullptr;
    _timeFeature = nullptr;
    _luxFeature = nullptr;
    _statusPipe = nullptr;

    const int core_id = 3;
    const pid_t pid = getpid();
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    _logger->logInfo("*** LightSystem Starting Up ***");

    const int set_result = sched_setaffinity(pid, sizeof(cpu_set_t), &cpuset);
    if (set_result != 0)
    {
        _logger->logInfo("*LightSystem Could Not Schedual Processor Affinity CPU 3");

    }

    const int get_affinity = sched_getaffinity(pid, sizeof(cpu_set_t), &cpuset);
    if (get_affinity != 0)
    {
      _logger->logInfo("*LightSystem Could Not Set Processor Affinity CPU 3");
      return;
    }

    if (CPU_ISSET(core_id, &cpuset))
    {
      _logger->logInfo("*LightSystem Successfully set thread to affinity to CPU 3");

    }
    else
    {
        _logger->logInfo("*LightSystem Failed to set thread to affinity to CPU 3");
        return;

    }

}


void LightSystem::stopFeatures()
{
    _logger->logInfo("LightSystem::stopFeatures()");
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



}

LightSystem::~LightSystem()
{


    stopSystem();
    _ledWrapper.clearLeds();

    _logger->logInfo("*** ~LightSystem Offline ***");

}

const char *LightSystem::getEnumName(int index)
{
    const char* _lightShowNames[] = {LIGHT_SHOWS(LIGHT_SHOWS_SHOW)};
    return _lightShowNames[index];
}

void LightSystem::startShows()
{

    if(_running) return;

    _running = true;
    runShow();

    _logger->logInfo("*** LightSystem::startShows Started ***");

}


void LightSystem::stopShows()
{
    if(false == _running) return;

     _logger->logInfo("LightSystem::stopShows Stopping shows");
   _running = false;

   if(_runningShows.count() > 0 && _runningShows[0] != nullptr)
        _runningShows[0]->stopShow();

   _ledWrapper.clearLeds();
   _logger->logInfo("LightSystem::stopShows Stopped shows");

}

void LightSystem::playPlayList(QString playList)
{

    QJsonDocument doc = QJsonDocument::fromJson(playList.toUtf8());
    _logger->logInfo("LightSystem::playPlayList");

    if(!doc.isNull() && doc.isArray())
    {

        QJsonArray jsonArray = doc.array();
        foreach (QJsonValue info, jsonArray)
        {
            QString msg = QJsonDocument::fromVariant(info.toVariant()).toJson(QJsonDocument::Compact);
            processMsgReceived(msg);
        }

    }
    else
    {
         _logger->logInfo("LightSystem::playPlayList Invalid Json");
    }
}


void LightSystem::playArtShow(QJsonObject jsonObject)
{
    PlayListManager pmanager;

    QJsonDocument doc = QJsonDocument::fromJson(pmanager.getArtShow(jsonObject).toUtf8());
    QJsonObject sc = doc.object();


    sc["sc"] = jsonObject.value("sc").toInt();
    sc["clearStart"] = jsonObject.value("clearStart");
    sc["clearFinish"] = jsonObject.value("clearFinish");

    doc.setObject(sc);
    QString artShow = doc.toJson(QJsonDocument::Compact);

    processMsgReceived(artShow);

}




void LightSystem::playuserPlayList(QJsonObject jsonObject)
{
    PlayListManager pmanager;
    QString playList = pmanager.getPlayList(jsonObject);
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

void LightSystem::sendSystemInfo()
{
    QJsonObject statusObject;
    QString payload;

    _logger->logInfo("LightSystem::sendSystemInfo");
    statusObject.insert("systemName", _settings->getSystemName());
    statusObject.insert("systemTemp", getSystemTemp());

    _runningShowsMutex.lock();
    statusObject.insert("showsInQueue", _runningShows.count());
    if(_runningShows.count() > 0)
        statusObject.insert("runningShow",_runningShows[0]->getShowName());
    _runningShowsMutex.unlock();

    getSysInfo(&statusObject);
    getVoltageInfo(&statusObject);
    QJsonDocument doc(statusObject);
    payload =  doc.toJson(QJsonDocument::Compact);
    _statusPipe->SendMessage((void*)payload.toStdString().c_str(), payload.length());


}

void LightSystem::processMsgReceived(QString msg)
{
 //  std::stringstream info;

//   info << "LightSystem::processMsgReceived: " << msg.toStdString().c_str();
 //  _logger->logInfo(info.str());

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
            else if(jsonObject.value("playPlaylist").toInt())
            {
                playuserPlayList(jsonObject);
            }
            else if(jsonObject.value("systemConfigChange").toInt())
            {
                _logger->logInfo("System Configuration Changes detected. Resetting System.");
               stopSystem();

               startSystem();

            }
            else if(jsonObject.value("clearQueue").toInt())
            {
                clearQueue();
            }
            else if(jsonObject.value("systemInfo").toInt())
            {
                sendSystemInfo();
            }
            else if(jsonObject.value("playArtShow").toInt())
            {
                playArtShow(jsonObject);
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


void LightSystem::processMsgReceivedTwitch(QString msg)
{
    std::stringstream info;

    info << "LightSystem::processMsgReceivedTwitch: " << msg.toStdString().c_str();
    _logger->logInfo(info.str());

    processMsgReceived(msg);

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

        case BBalls:
            _runningShows.append(new ShowBouncingBalls(&_ledWrapper, show, showParms));
            break;

        case Fade:
            _runningShows.append(new ShowFade(&_ledWrapper, show, showParms));
            break;

        case MatrixArtShow:
            _runningShows.append(new MatrixArt(&_ledWrapper, show, showParms));
            break;

        case MatrixText:
            _runningShows.append(new MatrixScrollText(&_ledWrapper, show, showParms));
            break;

        case DelayShow:
            _runningShows.append(new ShowDelay(&_ledWrapper, show, showParms));
            break;

        case MatrixTripShow:
            _runningShows.append(new MatrixTrip(&_ledWrapper, show, showParms));
            break;

        case MatrixC:
            _runningShows.append(new MatrixCircle(&_ledWrapper, show, showParms));
            break;

        case MatrixS:
            _runningShows.append(new MatrixSquare(&_ledWrapper, show, showParms));
            break;

        case MatrixT:
            _runningShows.append(new MatrixTriangle(&_ledWrapper, show, showParms));
            break;

        case MatrixJpg:
            _runningShows.append(new MatrixCreateJpg(&_ledWrapper, show, showParms));
            break;

        case MatrixDF:
            _runningShows.append(new MatrixCreateDisplayFile(&_ledWrapper, show, showParms));
             break;

        case MatrixVB:
            _runningShows.append(new MatrixVolBars(&_ledWrapper, show, showParms));
            break;

        case MatrixTimeS:
            _runningShows.append(new MatrixScrollTime(&_ledWrapper, show, showParms));
            break;

        case MatrixAntShow:
            _runningShows.append(new MatrixAntArt(&_ledWrapper, show, showParms));
            break;

        case MatrixKScope:
            _runningShows.append(new MatrixKaleidoscope(&_ledWrapper, show, showParms));
            break;

        case MatrixColorEvo:
            _runningShows.append(new MatricColorEvolution(&_ledWrapper, show, showParms));
        break;

        case MatrixGameLife:
            _runningShows.append(new MatrixGameOfLife(&_ledWrapper, show, showParms));
        break;

        case MatrixMaze:
            _runningShows.append(new MatrixMazeCreator(&_ledWrapper, show, showParms));
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

    _logger->logInfo("LightSystem::logShow");
    QSqlDatabase database = QSqlDatabase::database("rpiLightSystem");
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

        _runningShowsMutex.lock();
        _runningShows[0]->start();
        _runningShowsMutex.unlock();

    }
}


void LightSystem::cleanUpShow(ILightShow* show)
{
    std::stringstream info;

    if(show != nullptr) delete show;

    _runningShowsMutex.lock();
    _runningShows.removeOne(show);
    _runningShowsMutex.unlock();

    info << "LightSystem::cleanUpShow Show(" <<  show->getShowName().toStdString().c_str() << ") Queue(" << _runningShows.count() << ")";
    _logger->logInfo(info.str());

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
    _logger->logInfo("LightSystem::loadFeatures");
    std::stringstream info;

    QSqlDatabase database = QSqlDatabase::database("rpiLightSystem");
    database.setHostName(_settings->getInstance()->getDBServer());
    database.setUserName(_settings->getInstance()->getDBUser());
    database.setPassword(_settings->getInstance()->getDBPwd());
    database.setDatabaseName(_settings->getInstance()->getDataBase());
    if(database.open())
    {
        QString sql("select * from lightSystemFeatures where enabled = 1 and lightSystemId = ");
        sql.append(QString::number(_settings->getInstance()->getSystemId()));
        QSqlQuery qry = database.exec(sql);
        if(qry.lastError().type() == QSqlError::NoError)
        {

            while(qry.next())
            {

                int lightfeatureId = qry.value("featureId").toInt();
                switch((lightFeatures) lightfeatureId)
                {
                    case MotionSensor:
                    {
                        _logger->logInfo("Starting MotionLights Feature");
                        _motionFeature = new MotionLightsFeature(qry);
                        connect(_motionFeature, SIGNAL(motionLightsStateChange(MotionLightsFeature*, int)), this, SLOT(motionStateChange(MotionLightsFeature*, int)));
                        _motionFeature->start();
                    }
                    break;

                    case LightSensor:
                    {
                        _logger->logInfo("Starting LightSensor Feature");
                        _lightSensorFeature = new LightSensorFeature(qry);
                        connect(_lightSensorFeature, SIGNAL(lightSensorStateChange(LightSensorFeature*, int)), this, SLOT(lightStateChange(LightSensorFeature*, int)));
                        _lightSensorFeature->start();
                    }
                    break;

                    case TimeBased:
                    {
                        _logger->logInfo("Starting Time Feature");
                        _timeFeature = new TimeFeature(qry);
                        connect(_timeFeature, SIGNAL(timeStateChange(TimeFeature*, int)), this, SLOT(timeStateChange(TimeFeature*, int)));
                        _timeFeature->start();
                    }
                    break;

                    case LuxSensor:
                    {
                        _logger->logInfo("Starting LightLux Feature");
                        _luxFeature = new LightLuxFeature(qry, ContinueHigh);
                        connect(_luxFeature, SIGNAL(lightLuxStateChange(LightLuxFeature*, quint32)), this, SLOT(lightLuxStateChange(LightLuxFeature*, quint32)));
                        _luxFeature->start();
                    }
                    break;


                    default:
                      _logger->logInfo("Starting LightLux We should Not Be here, ever.");
                    break;



                }

            }


        }
        else
        {
            info << "loadFeatures error: " << qry.lastError().text().toStdString().c_str();
            _logger->logWarning(info.str());

        }
        database.close();
    }
    else
    {
         info << "loadFeatures error: " << database.lastError().text().toStdString().c_str();
         _logger->logWarning(info.str());
    }


}

bool LightSystem::startSystem()
{
    std::stringstream info;
    std::stringstream queue;
    unsigned int numLoops = 0;

    if(_started ) return true;


    while(_started == false && numLoops++ < 100)
    {
        info.str("");
	info << "Attempting To Load Settings: " << numLoops << " out of 100";
	
	_logger->logInfo(info.str());
	if(numLoops != 1)
		Ws2811Wrapper::waitMillSec(5000);
	_started = _settings->loadSettings();
    
    }
    if(_started)
    {
        _logger->logInfo("LightSystem::startSystem Initilizing Led channels");
        foreach(ChannelSettings* channel,  _settings->getChannels())
        {
            info.str("");
            info << "start() Settings Host(" << _settings->getHostName().toStdString().c_str() << ") Channel(" << channel->channelId() << ") DMA(" << channel->dma() << ") GPIO(" <<
            channel->gpio() << ") sType(" << channel->stripType() << ") Rows(" <<
            channel->stripRows() << ") Columns(" << channel->stripColumns() << ") Brightness(" <<
            channel->brightness() << ") MQTTBroker(" << _settings->getMqttBroker().toStdString().c_str() << ")";


            ws2811_return_t renderResults;
            _logger->logInfo(info.str());
            if( (LedStripType)channel->stripType() != MATRIX_2121)
            {
                renderResults = _ledWrapper.initStrip( (ws2811Channel)channel->channelId(), channel->stripRows(), channel->stripColumns(),
                                                           (LedStripType)channel->stripType(), channel->dma(), channel->gpio(), (matrixDirection)channel->matrixdirection());
            }
            else
            {
                renderResults = _ledWrapper.initStrip(channel->stripRows(), channel->stripColumns(), (LedStripType)channel->stripType(),
                                                      (matrixDirection)channel->matrixdirection(), (Wiring2121)channel->matrix2121Wiring());
            }

            if(renderResults != WS2811_SUCCESS)
            {

              info.str("");
              info << "LightSystem start() Failed Code(" << renderResults << ") Msg(" <<
                      _ledWrapper.getws2811ErrorString(renderResults) << ")";
              _logger->logWarning(info.str());
              _started = false;
              return _started;
            }
            _ledWrapper.setBrightness(channel->brightness());
            _ledWrapper.clearLeds();

        }

        _ledWrapper.setClearOnExit(true);

        _logger->logInfo("LightSystem::startSystem Setting Up MQTT");
        _mqq = new MqttReceiver(_settings->getMqttBroker(), _settings->getHostName(), 0);
        connect(_mqq, SIGNAL(msgReceived(QString)), this, SLOT(processMsgReceived(QString)));


        _logger->logInfo("LightSystem::startSystem Setting Up MQTT System Status");
        queue << _settings->getHostName().toStdString().c_str() << "/SystemStatus";
        _statusPipe = new MQTTPublisher(_settings->getMqttBroker().toStdString(),queue.str(),0, _settings->getMqttRetryDelay(), _settings->getMqttRetries());
        _statusPipe->Start();

        if(_settings->getTwitchSupport())
        {
            _logger->logInfo("LightSystem::startSystem Setting Up Twitch Support");
            queue.str("");
            queue << _settings->getHostName().toStdString().c_str() << "/" << _settings->getMqttTwitchQueue().toStdString().c_str();
            _twitch  = new MqttReceiver(_settings->getMqttBroker(), queue.str().c_str(), 0,queue.str().c_str());
            connect(_twitch, SIGNAL(msgReceived(QString)), this, SLOT(processMsgReceivedTwitch(QString)));

        }

        loadFeatures();
        startShows();
    }
    else
    {
        _logger->logWarning("Problem Loading Config File.");
    }

    return _started;
}

void LightSystem::stopSystem()
{
    if(false == _started) return;
    _logger->logInfo("**LightSystem::stopSystem Stopping");
    _started = false;
    _mqq->stop();
    delete _mqq;
    _mqq = nullptr;

    _statusPipe->Stop();
    _statusPipe = nullptr;

    if(nullptr != _twitch)
    {
        _twitch->stop();
        delete _twitch;
        _twitch = nullptr;
    }

    stopFeatures();
    clearQueue();

    _logger->logInfo("**LightSystem::stopSystem Stopped");
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
    int  brightness;

    if(lux <= feature->getLuxThreshHold())
    {
        if(_runningShows.count() == 0)
        {

                info << "LightSystem::lightLuxStateChange Run PlayList(" << feature->getFeaturePlayList() << ") Lux(" << lux << ") ThreshHold(" << feature->getLuxThreshHold() <<")";
                _logger->logInfo(info.str());

                PlayListManager pmanager;
                QString playList = pmanager.getPlayList(feature->getFeaturePlayList());
                playPlayList(playList);
                startShows();
        }
        else
        {
            info << "LightSystem::lightLuxStateChange Show Running Lux(" << lux << ") ThreshHold(" << feature->getLuxThreshHold() <<")";
            _logger->logInfo(info.str());

        }

    }

    lux = (lux > 255) ? 255 : lux;
    brightness = 255 - lux;
    brightness = (brightness <= 0) ? 1 : brightness;

    _ledWrapper.setBrightness(brightness);

}

double LightSystem::getSystemTemp()
{
    FILE *temperatureFile;
    double temp = 0.00;

    temperatureFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
    if (temperatureFile != nullptr)
    {
        fscanf (temperatureFile, "%lf C", &temp);
    }

    return  temp /= 1000;
}




void LightSystem::getVoltageInfo(QJsonObject *statusObject)
{
    int th;
    FILE *f;
    char buf[256];

    std::stringstream info;

    char tmpFile[] = "/tmp/rpiL_XXXXXX";

    int tmp = mkstemp(tmpFile);
    close(tmp);

    sprintf(buf,"vcgencmd get_throttled > %s",tmpFile);
        system(buf);

    if ((f = fopen(tmpFile,"r")) != nullptr)
    {
        fscanf(f,"throttled=%x", &th);
        fclose(f);

        if (th & 1<<0) info << "Under-voltage detected<br>";
        if (th & 1<<1) info << "Frequency capped<br>";
        if (th & 1<<2) info << "Currently throttled<br>";
        if (th & 1<<3) info << "Temperature limit active<br>";
        if (th & 1<<16) info << "Under-voltage occurred<br>";
        if (th & 1<<17) info << "Frequency cap occurred<br>";
        if (th & 1<<18) info << "Throttling occurred<br>";
        if (th & 1<<19) info << "Temperature limit occurred<br>";

    }

    if(info.str().length() > 0)
        statusObject->insert("alerts", info.str().c_str());
    else
        statusObject->insert("alerts", "None");
    unlink(tmpFile);
}

void LightSystem::getSysInfo(QJsonObject *statusObject)
{

    struct sysinfo s_info;
    std::stringstream info;
    const double megabyte = 1024 * 1024;

    int error = sysinfo(&s_info);
    if(error != 0)
    {
        info << "getSysInfo Could not get SysInfo! Error: " << error;
       _logger->logWarning(info.str());
        return;
    }

    long uptime = s_info.uptime;
    int days = uptime / 86400;
    int hours = (uptime / 3600) - (days * 24);
    int mins = (uptime / 60) - (days * 1440) - (hours * 60);
    info << "D: " << days << " H: " << hours << " M: " << mins << " S: " << s_info.uptime % 60;
    statusObject->insert("uptime", info.str().c_str());

    statusObject->insert("totalRam", QString::number(s_info.totalram / megabyte));
    statusObject->insert("freeRam", QString::number(s_info.freeram / megabyte));

    float f_load = 1.f / (1 << SI_LOAD_SHIFT);
    info.str("");
    info << "(1 min): " << std::setprecision(2) << s_info.loads[0] * f_load << " (%" << std::setprecision(2) << s_info.loads[0] * f_load * 100/get_nprocs() << " CPU)";    
    statusObject->insert("load", info.str().c_str());

}

//TWTICH
/*
LedLightShows LightSystem::getShowId(const QString& twitchId)
{

    const char* twitchNames[] = {LIGHT_SHOWS(LIGHT_SHOWS_TWITCH)};
    for(int counter = 0; counter < LIGHT_SHOWS_ENUM_COUNT; counter++)
    {
        if(!twitchId.compare(twitchNames[counter],Qt::CaseInsensitive))
        {
            return (LedLightShows)counter;
        }
    }

   return Nope;
}
*/

/* Keeping forever as it was good work but with twitch extensions (panel) we no longer need

QString LightSystem::parseTwitchCmd(const QStringList& showParms)
{

    QJsonObject showObject;
    QJsonObject colors;


    if(!showParms[0].compare("chgbrightness",Qt::CaseInsensitive))
    {
        showObject.insert("chgBrightness", showParms[1]);
        QJsonDocument doc(showObject);
        return doc.toJson(QJsonDocument::Compact);
    }


    if(!showParms[0].compare("clearqueue",Qt::CaseInsensitive))
    {
        showObject.insert("clearQueue", 1);
        QJsonDocument doc(showObject);
        return doc.toJson(QJsonDocument::Compact);
    }

    LedLightShows showId = getShowId(showParms[0]);
    if(showId != Nope)
    {

        showObject.insert("show", QJsonValue::fromVariant(QString::number((int)showId)));
        try
        {
            for(int counter = 1; counter < showParms.length(); counter++)
            {
                QStringList parm = showParms[counter].split('=');
                if(parm.length() == 2)
                {
                    if(!parm[0].compare("c1",Qt::CaseInsensitive))
                    {

                        quint32 color = std::stoul(parm[1].toStdString().c_str(), nullptr, 16);

                        QJsonObject rgb;
                        rgb.insert("r",QJsonValue::fromVariant(_ledWrapper.Red(color)));
                        rgb.insert("g",QJsonValue::fromVariant(_ledWrapper.Green(color)));
                        rgb.insert("b",QJsonValue::fromVariant(_ledWrapper.Blue(color)));

                        colors.insert("color1", rgb);

                     }

                    if(!parm[0].compare("c2",Qt::CaseInsensitive))
                    {


                        quint32 color = std::stoul(parm[1].toStdString().c_str(), nullptr, 16);

                        QJsonObject rgb;
                        rgb.insert("r",QJsonValue::fromVariant(_ledWrapper.Red(color)));
                        rgb.insert("g",QJsonValue::fromVariant(_ledWrapper.Green(color)));
                        rgb.insert("b",QJsonValue::fromVariant(_ledWrapper.Blue(color)));

                        colors.insert("color2", rgb);

                     }

                    if(!parm[0].compare("c3",Qt::CaseInsensitive))
                    {


                        quint32 color = std::stoul(parm[1].toStdString().c_str(), nullptr, 16);

                        QJsonObject rgb;
                        rgb.insert("r",QJsonValue::fromVariant(_ledWrapper.Red(color)));
                        rgb.insert("g",QJsonValue::fromVariant(_ledWrapper.Green(color)));
                        rgb.insert("b",QJsonValue::fromVariant(_ledWrapper.Blue(color)));

                        colors.insert("color3", rgb);

                     }

                    if(!parm[0].compare("c4",Qt::CaseInsensitive))
                    {


                        quint32 color = std::stoul(parm[1].toStdString().c_str(), nullptr, 16);

                        QJsonObject rgb;
                        rgb.insert("r",QJsonValue::fromVariant(_ledWrapper.Red(color)));
                        rgb.insert("g",QJsonValue::fromVariant(_ledWrapper.Green(color)));
                        rgb.insert("b",QJsonValue::fromVariant(_ledWrapper.Blue(color)));

                        colors.insert("color4", rgb);

                     }


                    if(!parm[0].compare("delay",Qt::CaseInsensitive))
                        showObject.insert("delay", parm[1]);

                    if(!parm[0].compare("minutes",Qt::CaseInsensitive))
                        showObject.insert("minutes", parm[1]);

                    if(!parm[0].compare("brightness",Qt::CaseInsensitive))
                        showObject.insert("brightness", parm[1]);

                    if(!parm[0].compare("width",Qt::CaseInsensitive))
                        showObject.insert("width", parm[1]);

                    if(!parm[0].compare("colorEvery",Qt::CaseInsensitive))
                        showObject.insert("colorEvery", parm[1]);

                    if(!parm[0].compare("gamma",Qt::CaseInsensitive))
                        showObject.insert("gamma", parm[1].toInt());

                    if(!parm[0].compare("clearstart",Qt::CaseInsensitive))
                        showObject.insert("clearStart", parm[1].toInt());

                    if(!parm[0].compare("clearfinish",Qt::CaseInsensitive))
                        showObject.insert("clearFinish", parm[1].toInt());

                }
            }

            if(colors.length() > 0)
                showObject.insert("colors", colors);

            QJsonDocument doc(showObject);
            return doc.toJson(QJsonDocument::Compact);

        }
        catch(...)
        {
            _logger->logCritical("Exception Parsing Parms!");
        }

    }

    return "";
}

//implment
if(msg.split(' ')[0] == "!lumawin")
    {
         QString show(msg.split(' ')[1]);

         QStringList showParms(show.split(','));

         QString msg = parseTwitchCmd(showParms);
         if(msg.length() > 0)
            processMsgReceived(msg);

    }
*/



//'#'+(Math.random()*0xFFFFFF<<0).toString(16);
