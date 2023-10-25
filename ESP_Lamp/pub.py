import paho.mqtt.client as paho
import time
import serial
import questionary

port = "COM3"
connection = serial.Serial(port, timeout=1)

broker = "broker.emqx.io"
client = paho.Client("client-1")

print("Connecting to broker", broker)
client.connect(broker)
client.loop_start()

def start_loop(topic):
    start_time, end_time = 20 , 40
    min_end_time = 30

    seconds=1
    current_end=end_time
    cmd = '0'
    while True:
        if seconds==1:
            print(f'Led {start_time}-{current_end} second')
        print(f'{seconds} send command is {cmd}')

        seconds+=1
        if seconds>=start_time and seconds<=current_end:
            cmd = '1'
        else:
            cmd = '0'

        if seconds>60:
            seconds=1
            current_end-=1

        if current_end<min_end_time:
            current_end=end_time
        
        client.publish(topic, cmd)

        time.sleep(1)

topic_name = questionary.text("Topic:").ask()
connection.write(topic_name.encode())
start_loop(topic_name)

