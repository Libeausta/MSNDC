import paho.mqtt.client as mqtt_client 
import random 
import time
import questionary
 
broker = "broker.emqx.io" 
 
def on_connect(client, userdata, flags, rc): 
    if rc == 0: 
        print("Connected to MQTT Broker!") 
    else: 
        print("Failed to connect, return code %d\n", rc) 
 
client = mqtt_client.Client(f'lab_{random.randint(10000, 99999)}') 
client.on_connect = on_connect 
client.connect(broker) 

min_time = 10  
led_time = 20 
while True: 
    state = "d" 
    client.publish("topic_esp/command", state) 
    print(f"publish state is {state}") 
    time.sleep(led_time) 
    led_time = max(min_time, led_time - 1) 
    
    state = "u" 
    client.publish("topic_esp/command", state) 
    print(f"publish state is {state}") 
    time.sleep(60 - led_time)
