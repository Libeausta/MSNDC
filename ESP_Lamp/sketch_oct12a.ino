#include "Config.h"
#include "Wifi.h"
#include "Server.h"
#include "MQTT.h"

void setup() {  
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  start_AP_mode();

  if (init_wifi(false)) {
    init_MQTT();
    server_init();
    Serial.println("Wifi is working");
  } else {
    Serial.println("Wifi is not working");
  }

  String topic_name = "topic_1";
  if(Serial.available() > 0){
    char topic_name = Serial.read();
  }
  String command_topic = topic_name + "/command";
  String state_topic = topic_name + "/state";
  mqtt_client.subscribe(command_topic.c_str());
  mqtt_client.publish(state_topic.c_str(), "connected to MQTT");
  Serial.println("See me at " + state_topic);
}

void loop() {
  server.handleClient();
  mqtt_client.loop();
  delay(100);
}
