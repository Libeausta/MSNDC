// Latch pin (ST_CP) of 74HC595 to pin 5
int latchPin = 5;
// Latch pin (SH_CP) of 74HC595 to pin 6
int clockPin = 3;
// Data pin (DS of 74HC595 to pin 6
int dataPin = 7;
// Make sure MR is HIGH (connect ot Vcc)

byte digits[10] = {
    B11011101, // 0
    B01010000, // 1
    B11001110, // 2
    B11011010, // 3
    B01010011, // 4
    B10011011, // 5
    B10111111, // 6
    B11010000, // 7
    B11011111, // 8
    B11111011  // 9
};

int min = 0;
int sec = 0;

void setup(){
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    digitalWrite(clockPin, LOW);
    Serial.begin(9600);
}

void loop(){
    if (Serial.available()){
        if (Serial.available() == 4){
            min = (int)(Serial.read() - '0') * 10 + (int)(Serial.read() - '0');
            sec = (int)(Serial.read() - '0') * 10 + (int)(Serial.read() - '0');
        }
    }
    if (sec >= 0 and sec < 60){
        digitalWrite(latchPin, LOW);

        sec++;
        if (min - 10 < 0){
            show_time(0);
            show_time(min);
        } else{
            show_time(min / 10);
            show_time(min % 10);
        }

        if (sec - 10 < 0){
            show_time(0);
            show_time(sec);
        } else{
            show_time(sec / 10);
            show_time(sec % 10);
        }
        if (sec == 60){
            min++;
            sec = 0;
        }
        digitalWrite(latchPin, HIGH);
        delay(1000);
    }
}

void show_time(int digit){
    if (digit < 0 or digit > 9){
        return;
    }
    shiftOut(dataPin, clockPin, LSBFIRST, digits[digit]);
}

void shift_and_set(bool val){
    digitalWrite(dataPin, val);
    digitalWrite(clockPin, HIGH);
    delay(10);
    digitalWrite(clockPin, LOW);
}
