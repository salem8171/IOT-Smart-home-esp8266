#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <RemoteDebug.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include "SerialMqtt.h"
#include "SimpleAnalogSensor.h"
#include "ElectricCylinder.h"
#include "Config.h"

String decodePayload(uint8_t* payload, unsigned int length)
{
    String ret = "";
    for (unsigned int i = 0; i < length; i++) ret += (char) payload[i];
    return ret;
}

// Declarations
RemoteDebug debug;
WiFiClient client;
PubSubClient mqttClient = PubSubClient(client);
SimpleAnalogSensor gas_sensor;
ElectricCylinder window;
// SerialMqtt serial_mqtt;

void setup()
{
    // Serial.begin(9600);

    // WiFi setup
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) ESP.restart();

    // WiFi utilities setup
    MDNS.begin(ESP_LOCAL_DOMAIN_NAME);
    ArduinoOTA.begin(true);
    debug.begin(ESP_LOCAL_DOMAIN_NAME);

    // Components setup
    gas_sensor.setup(GAS_SENSOR_PIN);
    window.setup(WINDOW_MOTOR_PIN1, WINDOW_MOTOR_PIN2, WINDOW_COURSE_DELAY);

    // MQTT setup
    mqttClient.setServer(MQTT_SERVER_DOMAIN, MQTT_SERVER_PORT);
    while (! mqttClient.connect(MQTT_CLIENT_ID)) delay(500);
    mqttClient.subscribe(MQTT_TOPIC_CMD_KITCHEN_WINDOW);

    // MQTT topic reveive callback
    mqttClient.setCallback([](char * _topic, uint8_t * _payload, unsigned int length)
    {
        String topic = _topic;
        String payload = decodePayload(_payload, length);
        
        if (topic.equals(MQTT_TOPIC_CMD_KITCHEN_WINDOW))
        {
            if (payload.equals(MQTT_PAYLOAD_CMD_KITCHEN_WINDOW_OPEN)) {
                debug.println("operning window");
                window.open();
            }
            if (payload.equals(MQTT_PAYLOAD_CMD_KITCHEN_WINDOW_CLOSE)) window.close();
        }
    });

    // Serial MQTT setup
    // serial_mqtt.setup(new SoftwareSerial(SOFTWARE_SERIAL_RX_PIN, SOFTWARE_SERIAL_TX_PIN));
    // serial_mqtt.setCallback([](SerialMqtt::MqttData data) {
    //     debug.print("topic: "); debug.print(SerialMqtt::getStringTopic(data));
    //     debug.print("payload: "); debug.println(data.payload);
    //     mqttClient.publish(SerialMqtt::getStringTopic(data).c_str(), String(data.payload).c_str());
    // });
}

// typedef enum TopicType {cmd, sensor} TopicType;
// typedef enum Location {kitchen, living_room, bed_room} Location;
// typedef enum TopicSpecification {motion, light, bulb} TopicSpecification;

// typedef struct MqttData
// {
//     TopicType topic_type;
//     Location location;
//     TopicSpecification topic_specification;
//     int payload;
// } MqttData;

// typedef union TransferContainer
// {
//     MqttData data;
//     byte dataMirror[sizeof(MqttData)];
// } TransferContainer;

void loop()
{
    // Handlers
    ArduinoOTA.handle();
    debug.handle();
    mqttClient.loop();
    window.handle();
    // serial_mqtt.handle();
    // debug.println("debugging");

    // Serial.print(0); Serial.print(".");
    // Serial.print(1); Serial.print(".");
    // Serial.print(2); Serial.print(".");
    // Serial.println(100);

    // TransferContainer recieve_container;
    // if( Serial.available() >= sizeof(MqttData))
    // {
    //     for (unsigned int i = 0; i < sizeof(MqttData); i++) recieve_container.dataMirror[i] = Serial.read();
    //     debug.print("topic type: "); debug.println(recieve_container.data.topic_type);
    //     debug.print("location: "); debug.println(recieve_container.data.location);
    //     debug.print("topic spec: "); debug.println(recieve_container.data.topic_specification);
    //     debug.print("payload: "); debug.println(recieve_container.data.payload);

    //     if (recieve_container.data.payload == 40000) debug.println("OK");
    // }

    // return;
    
    // Publish events
    mqttClient.publish(MQTT_TOPIC_SENSOR_KITCHEN_GAS, String(gas_sensor.getValue()).c_str());
}