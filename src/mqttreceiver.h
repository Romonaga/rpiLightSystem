#ifndef MqttReceiver_H
#define MqttReceiver_H

#include <QObject>
#include <QString>
#include "dnrlogger.h"
#include "mqttsubscriber.h"

class MqttReceiver : public QObject
{
    Q_OBJECT
public:
    explicit MqttReceiver(const QString &broker, const QString &topic, int qos, QObject *parent = nullptr);
    explicit MqttReceiver(const QString &broker, const QString &topic, int qos, const QString& clientID, QObject *parent = nullptr);





    ~MqttReceiver();
    void stop();

    void callbackFunction(mqtt::const_message_ptr msg);

private:
    QString _broker;
    QString _topic;
    int _qos;
    DNRLogger* _logger;
    MQTTSubscriber* _data;

public:

signals:
    void msgReceived(QString);
};

#endif // TESTME_H
