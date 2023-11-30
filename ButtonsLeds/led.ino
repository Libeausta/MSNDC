#define BUTTON_PIN 2 
#define LED_PIN 11
#define BOUNCE_TIME 50
#define PRESSED HIGH
     
int previous = 0;
volatile long int press_time = 0;
volatile bool pressed_candidate = false;
volatile long int hold_time = 0;
volatile int press_count = 0;
bool button_pressed = false;
bool led_bright = false;
int brightness = 255;
bool direction_up = true;


void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);
  attachInterrupt(0, process_button_click, RISING);
}


void loop() {
  int val = digitalRead(BUTTON_PIN);
  if(pressed_candidate == true and 
     millis() - press_time > BOUNCE_TIME) {
    if(digitalRead(BUTTON_PIN) == PRESSED){
      button_pressed = true;
    } else {
      pressed_candidate = false;
    }
  }


  if(button_pressed == true) {    
  	hold_time = millis() - press_time;
    Serial.print("Button is hold");
    Serial.println(hold_time);

    if(hold_time > 180 && led_bright){
      if (brightness < 255 and brightness > 0 and direction_up){
        brightness++;
      }
      else if (brightness < 255 and brightness > 0 and !direction_up){
        brightness--;
      }
      else if (brightness == 0){
        brightness++;
        direction_up = true;
      }
      else if (brightness == 255){
        brightness--;
        direction_up = false;
      }
      analogWrite(LED_PIN, brightness);
    }
    
  } 

  if(button_pressed == true and
    previous == PRESSED and
    val != PRESSED) {
    Serial.print("Button is released");
    button_pressed = false;
    pressed_candidate = false;
    
    if(hold_time <= 180){
      Serial.println("Button is clicked");
      if (led_bright) {
        digitalWrite(LED_PIN, LOW);
        brightness = 255;
      } else {
        digitalWrite(LED_PIN, HIGH);
      }

      led_bright = !led_bright;
    }
  }
  previous = val;
}

void process_button_click() {
  if (pressed_candidate == false) {
    press_time = millis();
    pressed_candidate = true;
    press_count++;
    hold_time = 0;
  }
}
