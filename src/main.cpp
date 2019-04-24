#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <RemoteDebug.h>
#include <PubSubClient.h>

RemoteDebug debug;
WiFiClient client;
PubSubClient mqttClient(client);

void setup()
{
    WiFi.begin("wifi_ssid", "wifi_pass");
    if (WiFi.waitForConnectResult() != WL_CONNECTED) ESP.restart();

    MDNS.begin("esp8266");
    ArduinoOTA.begin(true);
    debug.begin("esp8266");

    mqttClient.setServer("broker ip", 1883);
    mqttClient.setCallback([](char * topic, uint8_t * payload, unsigned int length){
        String message = "";
        for (int i = 0; i < length; i++) message += payload[i];
        debug.println(message);
    });
    while (! mqttClient.connect("esp8266")) delay(500);
}

void loop()
{
    ArduinoOTA.handle();
    debug.handle();
    mqttClient.loop();
    
    mqttClient.publish("topic", "Hello from esp8266");
    delay(1000);
}