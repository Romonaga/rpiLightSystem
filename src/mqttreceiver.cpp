#include "mqttreceiver.h"
#include <sstream>
#include <string>
#include <unistd.h>


MqttReceiver::MqttReceiver(const QString &broker, const QString &topic, int qos, QObject *parent) :
    QObject(parent), _broker(broker), _topic(topic), _qos(qos)
{
    char hostname[32];
    hostname[31] = '\0';
    gethostname(hostname, sizeof(hostname));
    init(hostname);

}


MqttReceiver::MqttReceiver(const QString &broker, const QString &topic, int qos, const QString& clientID, QObject *parent)
    : QObject(parent), _broker(broker), _topic(topic), _qos(qos)
{

    init(clientID);

}


void MqttReceiver::init(const QString& clientID)
{
    _logger = DNRLogger::instance();
    _settings = SystemSettings::getInstance();

    _data = new MQTTSubscriber(_broker.toStdString(), _topic.toStdString(), _qos, _settings->getMqttRetryDelay(), _settings->getMqttRetries());
    _data->setDebugLogging(true);


    _data->setPersistance(clientID.toStdString().c_str(), _qos);

    auto mqttSubCallBack = [](void *caller, mqtt::const_message_ptr msg)
    {
        ((MqttReceiver*)caller)->callbackFunction(msg);
    };

    _data->connectCallback(mqttSubCallBack, this);

    _data->start();
}


MqttReceiver::~MqttReceiver()
{
    stop();
    delete _data;

}

void MqttReceiver::stop()
{
    _data->stop();
}



void MqttReceiver::callbackFunction(mqtt::const_message_ptr msg)
{
    //std::stringstream info;
    if(msg)
    {
        //info << "MQTTSubscriber Message arrived" << std::endl;
        //info << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
        //info << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;
        emit msgReceived(msg->to_string().c_str());
    }
}
