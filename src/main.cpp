#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <RemoteDebug.h>
#include <PubSubClient.h>
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

void setup()
{
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
            if (payload.equals(MQTT_PAYLOAD_CMD_KITCHEN_WINDOW_OPEN)) window.open();
            if (payload.equals(MQTT_PAYLOAD_CMD_KITCHEN_WINDOW_CLOSE)) window.close();
        }
    });
}

void loop()
{
    // Handlers
    ArduinoOTA.handle();
    debug.handle();
    mqttClient.loop();
    window.handle();
    
    // Publish events
    mqttClient.publish(MQTT_TOPIC_SENSOR_KITCHEN_GAS, String(gas_sensor.getValue()).c_str());
}