#include <PubSubClient.h>

PubSubClient mqtt_client(wifi_client);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  Serial.print("Message is: ");
  for (int i=0; i<length; i++){
    Serial.print((char)payload[i]);
  }
  int state = (int)payload[0]-(int)'0';
  digitalWrite(led_pin, state);
}

void init_MQTT(){
  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setCallback(callback);
  while(!mqtt_client.connected()){
    Serial.println("Trying to connect:  ");
    Serial.println(mqtt_broker);
    String client_id = "esp8266_" + id();
    bool succses = mqtt_client.connect(client_id.c_str());
    if (succses) {
      Serial.println("Succsesfully connected with " + client_id);
    } else {
      Serial.println("Failed to connect with " + client_id);
      Serial.println(mqtt_client.state());
      delay(2000);
    }
  }
}