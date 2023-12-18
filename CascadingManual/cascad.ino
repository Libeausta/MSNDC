#include <SoftwareSerial.h>

#define START_PIN 2
#define END_PIN 9

SoftwareSerial next_node(10, 11);
int num_node = 1;
int ngb_node = 0;
int time = 0;
 
// 2 3 4 5 6 7 8 9 
// A B . C D E G F
bool digits[10][8] = {
  {1,1,0,1,1,1,0,1}, // 0
  {0,1,0,1,0,0,0,0}, // 1
  {1,1,0,0,1,1,1,0}, // 2
  {1,1,0,1,1,0,1,0}, // 3
  {0,1,0,1,0,0,1,1}, // 4
  {1,0,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1,1}, // 6
  {1,1,0,1,0,0,0,0}, // 7
  {1,1,0,1,1,1,1,1}, // 8
  {1,1,1,1,1,0,1,1}  // 9
};

void setup() {
  for (int i = START_PIN; i <= END_PIN; i++) {
  	pinMode(i, OUTPUT);
   	digitalWrite(i, HIGH);
  }
  Serial.begin(1200);
  next_node.begin(1200);
  while (Serial.available() > 0) {
    ngb_node = Serial.read();
  }
  Serial.print("Number of neighbours: ");
  Serial.print('1');
  while(next_node.available() == 0){
    delay(1);
    if (millis() - time == 10){
      break;
    }
  }
  while (next_node.available() > 0){ 
    delay(10);
    ngb_node = next_node.read();
    if(ngb_node == int(char('1'))){ 
      num_node += 1;
  	  Serial.print('1');
    }
  }
}

void loop() {
  if (Serial.available() >= num_node) {
    char user_input = Serial.read();
    int digit = (int)(user_input - '0');
    show_digit(digit);
    while (Serial.available() > 0) {
      next_node.write(Serial.read());
    }
  }
  delay(100);
  if (Serial.available() > 0 && Serial.available() < num_node){
    show_digit(0);
    for (int i = 1; i < num_node-Serial.available(); i++) {
      next_node.write('0');
    }
    while (Serial.available()) {
      next_node.write(Serial.read());
    } 
  }
}

void show_digit(int digit) {
  if (digit < 0 or digit > 9) {
    return;
  }
  for (int i = START_PIN; i <= END_PIN; i++) {
    digitalWrite(i, !digits[digit][i-START_PIN]);
  }
}