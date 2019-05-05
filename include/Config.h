#ifndef CONFIG
#define CONFIG

#define WIFI_SSID "IOT-Smart-home"
#define WIFI_PASSWORD "NXCJG3P6"

#define ESP_LOCAL_DOMAIN_NAME "esp8266"

#define MQTT_SERVER_DOMAIN "salem-laptop-pc.local"
#define MQTT_SERVER_PORT 1883

#define MQTT_CLIENT_ID "esp8266"

#define WINDOW_MOTOR_PIN1 4
#define WINDOW_MOTOR_PIN2 5
#define WINDOW_COURSE_DELAY 2000

#define SOFTWARE_SERIAL_RX_PIN 12
#define SOFTWARE_SERIAL_TX_PIN 13

#define GAS_SENSOR_PIN A0

// Publish topics
#define MQTT_TOPIC_SENSOR_KITCHEN_GAS "sensor/kitchen/gas"

// Subscription topics
#define MQTT_TOPIC_CMD_KITCHEN_WINDOW "cmd/kitchen/window"
#define MQTT_PAYLOAD_CMD_KITCHEN_WINDOW_OPEN "open"
#define MQTT_PAYLOAD_CMD_KITCHEN_WINDOW_CLOSE "close"

#endif