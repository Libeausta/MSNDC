#define DATA_PIN 2
#define DATA_LEVEL LOW
#define TU 100
#define LETTER_DUR 3
#define WORD_DUR 7
#define TIME 10

String encode[] = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};
char letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
long start_data = 0;
int curr = 0;
int arr = sizeof(letters) / sizeof(char);
bool input_letter = false;
bool end_data = false;
bool new_data = true;
bool idle = true;
int duration[20];
bool duration_lvl[20];

void setup(){
  Serial.begin(9600);
  pinMode(2, INPUT);
  attachInterrupt(0, reciever_time, CHANGE );
}

void loop(){
  reciever_idle();
  if (input_letter){
    recieve_letters();  
    input_letter = false;
  }
}

void recieve_letters(){
    if (duration[curr - 1] >= LETTER_DUR && duration_lvl[curr - 1] != DATA_LEVEL){
        String letter = "";
        for (int i = 0; i < curr; ++i){
            if (duration_lvl[i] == DATA_LEVEL){
                if (duration[i] == 1){
                    letter += '.';
                }
                else if (duration[i] == 3){
                    letter += '-';
                }
            }
        }
        if (new_data){
          Serial.print("Receive: ");
          Serial.print("\n");
          new_data = false;
        }
        if (duration[curr - 1] >= TIME){
            new_data = true;
        }
        else if (duration[curr - 1] >= WORD_DUR){
            end_data = true;
        }
        curr = 0;
        for (int i = 0; i < arr; ++i){
            if (encode[i] == letter){
                Serial.print(letters[i]);
            }
        }
        if (end_data){
            Serial.print(" ");
            end_data = false;
        }
    }
}

void reciever_time(){
  if (!idle){
    duration[curr] = round((millis() - start_data) / TU);
    duration_lvl[curr] = !digitalRead(DATA_PIN);
    ++curr;
    input_letter = true;
  }
  start_data = millis();
  idle = false;
}

void reciever_idle(){
  if (!idle && round((millis() - start_data) / TU) >= TIME){
    duration[curr] = TIME;
    duration_lvl[curr] = !DATA_LEVEL;
    ++curr;
    start_data = millis();
    idle = true;
    input_letter = true;
  }
}