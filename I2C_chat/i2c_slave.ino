#include <Wire.h>

unsigned char id_slave = '1';
int name_slave = getAddr(id_slave);
char last_slave = ' ';

struct MatchArr {
  char name;
  int addr;
};

MatchArr arr[] = {
  {'1', 1}, {'2', 2}, {'3', 3}
};


void setup() {
  Serial.begin(9600);
  Wire.begin(name_slave);
  Wire.onReceive(getMsg);
}

void loop() {
  if (Serial.available()) {
    String input_txt = Serial.readStringUntil('\n');
    if (input_txt.startsWith("send ")) {
      char to_name = input_txt.charAt(5);
      int to_addr = getAddr(to_name);
      if (to_addr != -1) {
        String message = input_txt.substring(7);
        sendMsg(to_addr, message);
        Serial.print("To: ");
        Serial.write(to_name);
        Serial.write("\n");
        Serial.print("Message: ");
        Serial.print(message);
        Serial.write("\nMessage sent\n");
      } else {
        Serial.println("There is no such name\n");
      }
    } else if (input_txt.startsWith("reply ")) {
      if (last_slave != ' ') {
        String replyMessage = input_txt.substring(6);
        sendMsg(getAddr(last_slave), replyMessage);
        Serial.println("Reply sent");
      } 
    }
  }
}

void sendMsg(int to_addr, String message) {
  Wire.beginTransmission(to_addr);
  Wire.write(id_slave);
  Wire.write(message.c_str());
  Wire.endTransmission();
}

int getAddr(char name) {
  for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
    if (arr[i].name == name) {
      return arr[i].addr;
    }
  }
  return -1;
}

void getMsg(int length) {
  if (Wire.available()) {
    char senderId = Wire.read();
    last_slave = senderId;
    Serial.print("From: ");
    Serial.write(senderId);
    Serial.write("\n");
    Serial.print("Message: ");
    while (Wire.available()) {
      char c = Wire.read();
      Serial.print(c);
    }  
    Serial.println();
  }
}