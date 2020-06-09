#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

#include <QString>



class SystemSettings
{

public:
    static SystemSettings* getInstance();

    bool loadSettings();
    bool settingsExists();

    QString getServer() const;
    void setServer(const QString &server);

    QString getDataBase() const;
    void setDataBase(const QString &dataBase);

    QString getUser() const;
    void setUser(const QString &user);

    QString getPwd() const;
    void setPwd(const QString &pwd);



    void loadSystemSettings();
    
    QString getSystemName() const;
    int getStripType() const;
    uint16_t getStripHeight() const;
    uint16_t getStripWidth() const;
    short getDma() const;
    short getGpio() const;

    QString getHostName() const;
    int getBrightness() const;

    QString getMqttBroker() const;

    bool getDbgLog() const;

    void setBrightness(int brightness);

private:
    SystemSettings();
    static SystemSettings* _instance;
    QString _server;
    QString _dataBase;
    QString _user;
    QString _pwd;
    QString  _mmConfFile;
    QString _hostName;
    QString _mqttBroker;


    //System Setings From DB

    QString _systemName;
    int _stripType;
    uint16_t _stripHeight;
    uint16_t _stripWidth;
    short _dma;
    short _gpio;
    int _brightness;
    bool _dbgLog;
};

#endif
