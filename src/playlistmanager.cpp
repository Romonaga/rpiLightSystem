#include "playlistmanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonArray>

#include <sstream>
#include "systemsettings.h"


PlayListManager::PlayListManager(QObject *parent) : QObject(parent)
{
    _settings = SystemSettings::getInstance();
    _logger = DNRLogger::instance();

}

PlayListManager::~PlayListManager()
{

}

bool PlayListManager::savePlayList(const QString &name, int32_t userId, const QVector<ILightShow *> &shows)
{
    bool retVal = false;
    QJsonArray array;

    QSqlDatabase database = QSqlDatabase().addDatabase("QMYSQL","playListManager");
    database.setHostName(_settings->getDBServer());
    database.setUserName(_settings->getDBUser());
    database.setPassword(_settings->getDBPwd());
    database.setDatabaseName(_settings->getDataBase());

    if(database.open())
    {
        std::stringstream sql;
        foreach (ILightShow* show, shows)
        {
            QJsonDocument doc = QJsonDocument::fromJson(show->getShowParms().toUtf8());
            array <<  doc.object();
        }

        sql << "insert into userPlaylist(userID, playlistName, showParms) values(" <<
               userId << ",'" << name.toStdString().c_str() << "','" <<
               QJsonDocument(array).toJson(QJsonDocument::Compact).toStdString().c_str() << "')" ;

        QSqlQuery result(sql.str().c_str(), database);
        if(result.lastError().type() != QSqlError::NoError)
        {
            _logger->logInfo(result.lastError().text().toStdString());

        }

        database.close();
        retVal = true;

    }
    else
    {
        _logger->logInfo(database.lastError().text().toStdString());

    }

  //  QSqlDatabase::removeDatabase("playListManager");
    return retVal;

}


QString PlayListManager::getPlayList(int32_t userId, int32_t playlistID)
{
    QString playList;

    QSqlDatabase database = QSqlDatabase().addDatabase("QMYSQL","playListManager");
    database.setHostName(_settings->getDBServer());
    database.setUserName(_settings->getDBUser());
    database.setPassword(_settings->getDBPwd());
    database.setDatabaseName(_settings->getDataBase());

    if(database.open())
    {
        std::stringstream sql;
        sql << "select showParms from userPlaylist where userID = " << userId << " and ID = " << playlistID;
        QSqlQuery result(sql.str().c_str(), database);
        if(result.lastError().type() == QSqlError::NoError)
        {
            result.next();
            playList = result.value("showParms").toString();


        }
        else
        {
            _logger->logInfo(result.lastError().text().toStdString());
        }
        database.close();
    }
    else
    {
        _logger->logInfo(database.lastError().text().toStdString());
    }

   // QSqlDatabase::removeDatabase("playListManager");
    return playList;
}

QString PlayListManager::getPlayList(int32_t playlistID)
{
    QString playList;

    QSqlDatabase database = QSqlDatabase().addDatabase("QMYSQL","playListManager");
    database.setHostName(_settings->getDBServer());
    database.setUserName(_settings->getDBUser());
    database.setPassword(_settings->getDBPwd());
    database.setDatabaseName(_settings->getDataBase());

    if(database.open())
    {
        std::stringstream sql;
        sql << "select showParms from userPlaylist where ID = " << playlistID;
        QSqlQuery result(sql.str().c_str(), database);
        if(result.lastError().type() == QSqlError::NoError)
        {
            result.next();
            playList = result.value("showParms").toString();

        }
        else
        {
            _logger->logInfo(result.lastError().text().toStdString());
        }
        database.close();
    }
    else
    {
        _logger->logInfo(database.lastError().text().toStdString());
    }

   // QSqlDatabase::removeDatabase("playListManager");
    return playList;
}


bool PlayListManager::editPlayList(QJsonObject playList)
{
   bool retVal = false;
   QSqlDatabase database = QSqlDatabase().addDatabase("QMYSQL","playListManager");
   database.setHostName(_settings->getDBServer());
   database.setUserName(_settings->getDBUser());
   database.setPassword(_settings->getDBPwd());
   database.setDatabaseName(_settings->getDataBase());

   int playListID = playList["PlayList"].toString().toInt();
   QString jsonContainer =  playList["jsonContainer"].toString();
   if(database.open())
   {
       std::stringstream sql;
       sql << "update userPlaylist set showParms='" << jsonContainer.toStdString().c_str() << "' where ID = " << playListID;
       QSqlQuery result(sql.str().c_str(), database);
       if(result.lastError().type() != QSqlError::NoError)
       {
           _logger->logInfo(result.lastError().text().toStdString());

       }
       retVal = true;
       database.close();
   }
   else
   {
       _logger->logInfo(database.lastError().text().toStdString());
   }
   return retVal;

}
bool PlayListManager::deletePlayList(int32_t userId, int32_t playlistID)
{
    bool retVal = false;

    QSqlDatabase database = QSqlDatabase().addDatabase("QMYSQL","playListManager");
    database.setHostName(_settings->getDBServer());
    database.setUserName(_settings->getDBUser());
    database.setPassword(_settings->getDBPwd());
    database.setDatabaseName(_settings->getDataBase());

    if(database.open())
    {
        std::stringstream sql;
        sql << "delete from userPlaylist where userID = " << userId << " and ID = " << playlistID;
        QSqlQuery result(sql.str().c_str(), database);
        if(result.lastError().type() != QSqlError::NoError)
        {
            _logger->logInfo(result.lastError().text().toStdString());

        }
        retVal = true;
        database.close();
    }
    else
    {
        _logger->logInfo(database.lastError().text().toStdString());
    }
   // QSqlDatabase::removeDatabase("playListManager");
    return retVal;
}
