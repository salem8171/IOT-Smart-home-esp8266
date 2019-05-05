#include <Arduino.h>
#include <SoftwareSerial.h>

#ifndef SERIAL_MQTT
#define SERIAL_MQTT

class SerialMqtt
{
    // Types definitions
    public:
        typedef enum TopicType {cmd, sensor} TopicType;
        typedef enum Location {kitchen, living_room, bed_room} Location;
        typedef enum TopicSpecification {motion, light, bulb} TopicSpecification;

        typedef struct MqttData
        {
            TopicType topic_type;
            Location location;
            TopicSpecification topic_specification;
            int payload;
        } MqttData;
    
    private:
        typedef union TransferContainer
        {
            MqttData data;
            byte dataMirror[sizeof(MqttData)];
        } TransferContainer;

        static const String topic_type_string[];
        static const String location_string[];
        static const String topic_specification_string[];

    // Attributes and methods
    private:
        SoftwareSerial* serial;
        bool is_available;
        TransferContainer send_container;
        TransferContainer recieve_container;
        void (*callback)(MqttData);
    
    public:
        void setup(SoftwareSerial*);
        void connect();
        void send(MqttData);
        bool isAvailable();
        MqttData receive();
        void setCallback(void (*)(MqttData));
        void handle();
        static String getStringTopic(MqttData);
};

const String SerialMqtt::topic_type_string[] = { "cmd", "sensor" };
const String SerialMqtt::location_string[] = { "kitchen", "livingroom", "bedroom" };
const String SerialMqtt::topic_specification_string[] = { "motion", "light", "bulb" };

void SerialMqtt::setup(SoftwareSerial* serial)
{
    this -> serial = serial;
    serial -> begin(9600);
}

void SerialMqtt::connect()
{
    serial -> println("AT+CONA001583312FFD");
    while(serial -> available() < 9);
    for (int i = 0; i < 9; i++) serial -> read();
}

void SerialMqtt::send(MqttData mqtt_data)
{
    send_container.data.topic_type = mqtt_data.topic_type;
    send_container.data.location = mqtt_data.location;
    send_container.data.topic_specification = mqtt_data.topic_specification;
    send_container.data.payload = mqtt_data.payload;

    for (unsigned int i = 0; i < sizeof(MqttData); i++) serial -> write(send_container.dataMirror[i]);
}

bool SerialMqtt::isAvailable()
{
    return serial -> available() >= sizeof(MqttData);
}

SerialMqtt::MqttData SerialMqtt::receive()
{
    for (unsigned int i = 0; i < sizeof(MqttData); i++) recieve_container.dataMirror[i] = serial -> read();
    return recieve_container.data;
}

void SerialMqtt::setCallback(void (*callback)(MqttData))
{
    this -> callback = callback;
}

void SerialMqtt::handle()
{
    if (isAvailable()) callback(receive());
}

String SerialMqtt::getStringTopic(SerialMqtt::MqttData data)
{
    return topic_type_string[data.topic_type] + '/' + location_string[data.location] + '/' + topic_specification_string[data.topic_specification];
}

#endif