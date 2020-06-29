#include <unistd.h>

#include "systemsettings.h"
#include <QSettings>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


SystemSettings* SystemSettings::_instance = nullptr;

SystemSettings::SystemSettings()
{
    _mmConfFile =  "/etc/rpilightsystem.conf";

}

uint16_t SystemSettings::getTimePlayList() const
{
    return _timePlayList;
}

QString SystemSettings::getTimeFeatureEnd() const
{
    return _timeFeatureEnd;
}

QString SystemSettings::getTimeFeatureStart() const
{
    return _timeFeatureStart;
}

bool SystemSettings::getUseTimeFeature() const
{
    return _useTimeFeature;
}

short SystemSettings::getLightFeatureGpio() const
{
    return _lightFeatureGpio;
}

short SystemSettings::getMotionFeatureGpio() const
{
    return _motionFeatureGpio;
}

uint16_t SystemSettings::getLightPlayList() const
{
    return _lightPlayList;
}

uint16_t SystemSettings::getMotionPlayList() const
{
    return _motionPlayList;
}

uint16_t SystemSettings::getMotionDelayOff() const
{
    return _motionDelayOff;
}

bool SystemSettings::getUseLightFeature() const
{
    return _useLightFeature;
}

bool SystemSettings::getUseMotionFeature() const
{
    return _useMotionFeature;
}

bool SystemSettings::getLogShows() const
{
    return _logShows;
}

void SystemSettings::setBrightness(int brightness)
{
    _brightness = brightness;
}

bool SystemSettings::getDbgLog() const
{
    return _dbgLog;
}

QString SystemSettings::getMqttBroker() const
{
    return _mqttBroker;
}

QString SystemSettings::getHostName() const
{
    return _hostName;
}

short SystemSettings::getGpio() const
{
    return _gpio;
}

short SystemSettings::getDma() const
{
    return _dma;
}

uint16_t SystemSettings::getStripWidth() const
{
    return _stripWidth;
}

uint16_t SystemSettings::getStripHeight() const
{
    return _stripHeight;
}

int SystemSettings::getStripType() const
{
    return _stripType;
}

QString SystemSettings::getSystemName() const
{
    return _systemName;
}

bool SystemSettings::settingsExists()
{
    return QFileInfo(_mmConfFile).exists();
}

int SystemSettings::getSystemId() const
{
    return _systemId;
}

bool SystemSettings::getUseGammaCorrection() const
{
    return _useGammaCorrection;
}

void SystemSettings::loadSystemSettings()
{
   
    QSqlDatabase database = QSqlDatabase().addDatabase("QMYSQL","rpiLightSystem");

    database.setHostName(_server);
    database.setUserName(_user);
    database.setPassword(_pwd);
    database.setDatabaseName(_dataBase);
    if(database.open())
    {
        char name[100];
        gethostname( name, sizeof(name));
        _hostName = name;
        QString sql("select * from lightSystems where serverHostName = '");
        sql.append(_hostName);
        sql.append("'");
        QSqlQuery qry = database.exec(sql);
        if(qry.lastError().type() == QSqlError::NoError)
        {
            qry.next();
            _systemId = qry.value("ID").toInt();
            _systemName = qry.value("systemName").toString();
            _stripType = qry.value("stripType").toUInt();
            _stripHeight = qry.value("stripHeight").toUInt();
            _stripWidth = qry.value("stripWidth").toUInt();
            _dma = qry.value("dma").toInt();
            _gpio = qry.value("gpio").toInt();
            _brightness = qry.value("brightness").toInt();
            _useMotionFeature = qry.value("useMotionFeature").toBool();
            _useLightFeature = qry.value("useLightFeature").toBool();
            _useTimeFeature = qry.value("useTimeFeature").toBool();
            _motionDelayOff = qry.value("motionDelayOff").toUInt();
            _motionPlayList = qry.value("motionPlayList").toUInt();
            _lightPlayList = qry.value("lightPlayList").toUInt();
            _timePlayList = qry.value("timePlayList").toUInt();
            _motionFeatureGpio = qry.value("motionFeatureGpio").toInt();
            _lightFeatureGpio = qry.value("lightFeatureGpio").toInt();

            _timeFeatureStart = qry.value("timeFeatureStart").toString();
            _timeFeatureEnd = qry.value("timeFeatureEnd").toString();
            _useGammaCorrection = qry.value("useGammaCorrection").toBool();


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

bool SystemSettings::loadSettings()
{
    if(settingsExists())
    {
        QSettings settings(_mmConfFile, QSettings::IniFormat);
        if(settings.status() == QSettings::NoError)
        {
            settings.beginGroup("DATABASE");
            _server = settings.value("DBServer","").toString();
            _user = settings.value("DBUserID","").toString();
            _pwd = settings.value("DBPassword","").toString();
            _dataBase = settings.value("DataBase","").toString();
            settings.endGroup();

            settings.beginGroup("SETTINGS");

            _mqttBroker = settings.value("MQTTBroker","").toString();
            _dbgLog = settings.value("DBGLOG",false).toBool();
            _logShows = settings.value("LOGSHOWS", false).toBool();
            settings.endGroup();

            loadSystemSettings();

            return true;
        }
    }
    else
    {
        fprintf(stderr,"File Does Not Exists");
    }

    return false;
}

QString SystemSettings::getPwd() const
{
    return _pwd;
}

void SystemSettings::setPwd(const QString &pwd)
{
    _pwd = pwd;
}

QString SystemSettings::getUser() const
{
    return _user;
}

void SystemSettings::setUser(const QString &user)
{
    _user = user;
}

QString SystemSettings::getDataBase() const
{
    return _dataBase;
}

void SystemSettings::setDataBase(const QString &dataBase)
{
    _dataBase = dataBase;
}

QString SystemSettings::getServer() const
{
    return _server;
}
int SystemSettings::getBrightness() const
{
    return _brightness;
}

void SystemSettings::setServer(const QString &server)
{
    _server = server;
}

SystemSettings* SystemSettings::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new SystemSettings();
    }

    return _instance;
}
