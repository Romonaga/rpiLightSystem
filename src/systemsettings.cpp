#include <unistd.h>

#include "systemsettings.h"
#include <QSettings>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <sstream>

SystemSettings* SystemSettings::_instance = nullptr;

SystemSettings::SystemSettings()
{
    _mmConfFile =  "/etc/rpilightsystem.conf";


    _logger = DNRLogger::instance();
    _logger->setDebugOut(true);


}

int SystemSettings::getMatrix2121Wiring() const
{
    return _matrix2121Wiring;
}

QString SystemSettings::getUserArtDirectory() const
{
    return _userArtDirectory;
}

QMap<int, ChannelSettings *> SystemSettings::getChannels() const
{
    return _channels;
}


QString SystemSettings::getMqttTwitchQueue() const
{
    return _mqttTwitchQueue;
}

int SystemSettings::getMqttRetryDelay() const
{
    return _mqttRetryDelay;
}

int SystemSettings::getMqttRetries() const
{
    return _mqttRetries;
}

bool SystemSettings::getTwitchSupport() const
{
    return _twitchSupport;
}


bool SystemSettings::getLogShows() const
{
    return _logShows;
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


bool SystemSettings::loadSystemSettings()
{

    QSqlDatabase database;
    if(!QSqlDatabase::contains("rpiLightSystem"))
        database =  QSqlDatabase::addDatabase("QMYSQL","rpiLightSystem");
    else
        database = QSqlDatabase::database("rpiLightSystem");

    database.setHostName(_server);
    database.setUserName(_user);
    database.setPassword(_pwd);
    database.setDatabaseName(_dataBase);


    bool retVal =  false;
    std::stringstream info;



    if(database.open())
    {
        char name[100];
        gethostname( name, sizeof(name));
        _hostName = name;


        QString sql("SELECT * from lightSystems where enabled = 1 and serverHostName = '");
        sql.append(_hostName);
        sql.append("'");
        QSqlQuery qry = database.exec(sql);

        if(qry.lastError().type() == QSqlError::NoError && qry.numRowsAffected() > 0)
        {
            qry.next();
            _systemId = qry.value("ID").toInt();
            _systemName = qry.value("systemName").toString();
            _twitchSupport = qry.value("twitchSupport").toBool();
            _mqttRetries = qry.value("mqttRetries").toInt();
            _mqttRetryDelay = qry.value("mqttRetryDelay").toInt();
            _mqttTwitchQueue = qry.value("twitchMqttQueue").toString();
            _userArtDirectory = qry.value("userArtDirectory").toString();
            _logShows = qry.value("logShows").toBool();

            sql = "SELECT * FROM LedLightSystem.lightSystemChannels where enabled = 1 and lightSystemId = ";
            sql.append(QString().number(_systemId));

            QSqlQuery chanQry = database.exec(sql);
            if(chanQry.lastError().type() == QSqlError::NoError && chanQry.numRowsAffected() > 0)
            {
                while(chanQry.next())
                {
                    ChannelSettings* cSettings = new ChannelSettings(chanQry);

                    _channels.insert(cSettings->channelId(), cSettings);

                }

                retVal = true;

            }
            else
            {

                info << "loadSystemSettings Cant Load channel Settings: " << chanQry.lastError().text().toStdString().c_str();
                _logger->logInfo(info.str());
            }

        }
        else
        {
           info << "loadSystemSettings Cant Load Settings: " << qry.lastError().text().toStdString().c_str();
           _logger->logInfo(info.str());
        }
        database.close();
    }
    else
    {
        info << "loadSystemSettings Cant Load Settings: " << database.lastError().text().toStdString().c_str();
        _logger->logInfo(info.str());

    }

    return retVal;
}

bool SystemSettings::loadSettings()
{
    bool retVal = false;
    std::stringstream info;

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
            settings.endGroup();

            retVal = loadSystemSettings();

        }
    }
    else
    {
        info << "loadSystemSettings configuration file does not exist: " << _mmConfFile.toStdString().c_str() << "!";
        _logger->logInfo(info.str());
    }

    return retVal;
}

QString SystemSettings::getDBPwd() const
{
    return _pwd;
}

void SystemSettings::setPwd(const QString &pwd)
{
    _pwd = pwd;
}

QString SystemSettings::getDBUser() const
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

QString SystemSettings::getDBServer() const
{
    return _server;
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
