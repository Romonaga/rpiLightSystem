#include "lightluxfeature.h"
#include <QDebug>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

extern "C"
{
    #include <linux/i2c-dev.h>
    #include <i2c/smbus.h>
    #include <linux/i2c.h>
    #include <sys/ioctl.h>
}





LightLuxFeature::LightLuxFeature(const QSqlQuery& qry, BH1750FVI cmd)
{
    _logger = DNRLogger::instance();
    _settings = SystemSettings::getInstance();
    _lastLux = -1;
    _cmd = cmd;

    _featurePlayList = qry.value("featurePlayList").toUInt();
    _gpioPin = qry.value("featureGpio").toUInt();
    _luxThreshHold = qry.value("luxThreshHold").toUInt();

    _logger->logInfo("LightLuxFeature Init.");


}

LightLuxFeature::~LightLuxFeature()
{
    stop();
    _logger->logInfo("LightLuxFeature Shutting Down.");
}

void LightLuxFeature::run()
{

    int i2cfd;
    int retCode;

    uint32_t res;
    uint32_t lux;
    char buf[2];


    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);

     _logger->logInfo("LightLuxFeature run.");

     if ((i2cfd = open("/dev/i2c-1", O_RDWR)) < 0)
     {
         _logger->logCritical("LightLuxFeature i2c open error");
         return;
     }

     if (ioctl(i2cfd, I2C_SLAVE, BH1750FVI_I2C_ADDRESS) < 0)
     {
         _logger->logCritical("LightLuxFeature ioctl error");
         close(i2cfd);
         return;
     }

     retCode = i2c_smbus_write_byte(i2cfd, PowerOn);
     if (retCode < 0)
     {
         _logger->logCritical("LightLuxFeature PowerOn error.");
         close(i2cfd);
         return;
     }

     retCode = i2c_smbus_write_byte(i2cfd, _cmd);
     if (retCode < 0)
     {
         _logger->logCritical("LightLuxFeature ContinueHigh error\n");
         close(i2cfd);
         return;
     }

    _running = true;

    while(_conditionVar.wait_for(lck,std::chrono::milliseconds(300)) == std::cv_status::timeout && _running == true) //todo remove hardcoded wait_for
    {

        read(i2cfd, buf, 2);
        res = buf[0]*256+buf[1];
        lux = res / 1.2;
        if(_lastLux != lux)
        {
            _lastLux = lux;
            emit lightLuxStateChange(this, _lastLux);
        }
    }

    retCode=i2c_smbus_write_byte(i2cfd, PowerDown);
    close(i2cfd);

    _running = false;
    emit lightLuxCompleted(this);

}

void LightLuxFeature::stop()
{
    if(_running)
    {
        _running = false;
        _conditionVar.notify_all();
        wait();
        _logger->logInfo("LightLuxFeature Stopped.");
    }
}

uint32_t LightLuxFeature::getFeaturePlayList() const
{
    return _featurePlayList;
}

uint32_t LightLuxFeature::getLuxThreshHold() const
{
    return _luxThreshHold;
}


