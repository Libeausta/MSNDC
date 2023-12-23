#define DATA_PIN 9
#define DATA_LEVEL LOW
#define TU 100
#define LETTER_DUR 3
#define WORD_DUR 7

String encode[] = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};
char letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
int arr = sizeof(letters) / sizeof(char);
bool input_time = true;
char letter = ' ';

void setup(){
  Serial.begin(9600);
  pinMode(DATA_PIN, OUTPUT);
  digitalWrite(DATA_PIN, !DATA_LEVEL);
}

void loop(){
  if (Serial.available() >= 1){
    if (input_time){
        input_time = false;
        Serial.print("Send: ");
        Serial.print("\n");
    }
    letter = Serial.read();
    if (letter == ' '){
        send_letters();
    }
    else{
        for (int i = 0; i < arr; ++i){
            if (letter == letters[i]){
            Serial.print(letters[i]);
                send_msg(encode[i]);
                break;
            }
        }
    }
  }
  else if (!input_time){
    send_letters();
    input_time = true;
  }
}

void send_msg(String msg){
  int lng = msg.lnggth();
  for (int i = 0; i < lng; ++i){
    if (msg[i] == '.'){
      digitalWrite(DATA_PIN, DATA_LEVEL);
      delay(TU);
    }
    else{
      digitalWrite(DATA_PIN, DATA_LEVEL);
      delay(3 * TU);
    }
    digitalWrite(DATA_PIN, !DATA_LEVEL);
    delay(TU);
  }
  delay(2 * TU);
}

void send_letters(){
    digitalWrite(DATA_PIN, !DATA_LEVEL);
    delay((WORD_DUR - LETTER_DUR) * TU);
    Serial.print(' ');
}