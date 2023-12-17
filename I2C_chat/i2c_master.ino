#include <Wire.h>

int msg_slave = 0;
unsigned char id_slave = 1;

struct MatchArr {
  char name;
  int addr;
};

MatchArr arr[] = {
  {'1', 1}, {'2', 2}, {'3', 3}
};

void setup() {
  Serial.begin(9600);   
  Wire.begin(id_slave);
  Wire.onReceive(getMsg);
}

void loop() {
  if (Serial.available()) {
    char to_name = Serial.read();
    int to_addr = -1;
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
      if (arr[i].name == to_name) {
        to_addr = arr[i].addr;
        break;
      }
    }

    if (to_addr != -1) {
      Serial.print("To: ");
      Serial.write(to_name);
      Serial.write("\n");
      Serial.print("Message: ");
      Wire.beginTransmission(to_addr);
      delay(100);
      Wire.write(id_slave);
      while (Serial.available()) {
        char value = Serial.read();
        Serial.write(value);
        Wire.write(value);
      }
      Wire.endTransmission();
      Serial.write("\nMessage sent\n");
    } 
    else {
      Serial.println("There is no such name\n");
    }
  }
}

void getMsg(int length) {
  msg_slave = Wire.read();
  Serial.write("From: ");
  Serial.write(getName(msg_slave));
  Serial.write("\n");
  Serial.write("Message: ");
  while (Wire.available()) {
    char value = Wire.read();
    Serial.print(value);
  }  
  Serial.println();     
}

char getName(int addr) {
  for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
    if (arr[i].addr == addr) {
      return arr[i].name;
    }
  }
  return addr;
}
