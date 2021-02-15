#include <EEPROM.h>

#define LED_INT   PD4   // internal led
#define LED_WARM  PA3   // warm white leds
#define LED_COLD  PD3   // cold white leds
#define ENC_1     PA1   // encoder pin 1
#define ENC_2     PA2   // encoder pin 2
#define ENC_BTN   PB5   // encoder button pin

#define TEMPERATURE_STEPS   10    // temperature crossover factor
#define BRIGHTNESS_STEPS    5     // brightnes factor
#define MIN_BRIGHTNESS 1          // minimal lamp brightness
#define MAX_BRIGHTNESS 255        // maximal lamp brightness


uint8_t brightness = 64;
uint8_t temperature = 128;
uint8_t warm_value = MIN_BRIGHTNESS;
uint8_t cold_value = MIN_BRIGHTNESS;

uint8_t enc_pin_last = LOW;
uint8_t encoder_state = LOW;
bool encoder_has_changed = false;

int button_last_state = LOW;
int button_last_flickerable_state = LOW;
int button_state = LOW;
unsigned long button_last_debounce_time = 0;

bool lamp_on = true;

void blink_internal_led(){
  uint8_t val = digitalRead(LED_INT);
  digitalWrite(LED_INT, !val);
  delay(100);
  digitalWrite(LED_INT, val);
  delay(100);
  digitalWrite(LED_INT, !val);
  delay(100);
  digitalWrite(LED_INT, val);
}

void toggle_lamp(){
  if(lamp_on) 
    turn_lamp_off();
  else
    turn_lamp_on();
}

void turn_lamp_on(){
  if(!lamp_on){ // is off, turn on
    blink_internal_led();
  }
  lamp_on = true;
}

void turn_lamp_off(){
  if(lamp_on){  // is on, turn off
    EEPROM_write(0, brightness);    // save brightness to first byte in eeprom 
    EEPROM_write(1, temperature);   // save temperature to first byte in eeprom
    blink_internal_led();
  }
  lamp_on = false;
}

void add_brightness(bool direction){
    if(direction){
      if(brightness <= MAX_BRIGHTNESS - BRIGHTNESS_STEPS)
        brightness += BRIGHTNESS_STEPS;
      else
        blink_internal_led();
    }
    else{
      if(brightness >= MIN_BRIGHTNESS + BRIGHTNESS_STEPS)
        brightness -= BRIGHTNESS_STEPS;
      else if(brightness > 0)
        turn_lamp_off();
    }
    
}

void add_temperature(bool direction){
    if(direction){
      if(temperature <= 255 - TEMPERATURE_STEPS)
        temperature += TEMPERATURE_STEPS;
      else
        blink_internal_led();
    }
    else{
      if(temperature >= TEMPERATURE_STEPS)
        temperature -= TEMPERATURE_STEPS;
      else
        blink_internal_led();  
    }
}

void set_temperature(){
  warm_value = map(temperature,0,255,0,brightness);
  cold_value = map(255-temperature,0,255,0,brightness);
}

void set_lamp_leds(){
  analogWrite(LED_WARM, warm_value);
  analogWrite(LED_COLD, cold_value);
}

void encoder_callback(bool direction, bool btn_pressed){
  if(lamp_on){
    if(btn_pressed){
       add_temperature(!direction);
    }
    else{
      add_brightness(direction);
    }
  }
  else{
    if(direction)
      turn_lamp_on();
  }
  encoder_has_changed = true;
  set_temperature();
}

void setup() {

  FLASH_SetLowPowerMode(FLASH_LPMODE_POWERDOWN);
  // define pin modes
  pinMode(LED_INT, OUTPUT);
  pinMode(LED_WARM, OUTPUT);
  pinMode(LED_COLD, OUTPUT);
  pinMode(ENC_1, INPUT);
  pinMode(ENC_2, INPUT);
  pinMode(ENC_BTN, INPUT_PULLUP);

  // read from eeprom on power loss
  brightness = EEPROM_read(0);
  temperature = EEPROM_read(1);


}

void loop() {

  // read button/encoder
  button_state = digitalRead(ENC_BTN);
  encoder_state = digitalRead(ENC_1);

  // encoder
  if ((enc_pin_last == LOW) && (encoder_state == HIGH)) {
    encoder_callback(!digitalRead(ENC_2), !button_state);
  }
  enc_pin_last = encoder_state;

  // button
  if (button_state != button_last_flickerable_state) {
    button_last_debounce_time = millis();
    button_last_flickerable_state = button_state;
  }

  if ((millis() - button_last_debounce_time) > 50) {
    digitalWrite(LED_INT, button_state);
    if(button_last_state == HIGH && button_state == LOW)
      encoder_has_changed = false;

    else if(button_last_state == LOW && button_state == HIGH){
      if(!encoder_has_changed){
        toggle_lamp();
        encoder_has_changed = false;
      }
    }
    button_last_state = button_state;
  }

  // lamp
  if(lamp_on){
    set_lamp_leds();
  }
  else{
    analogWrite(LED_COLD, 0);
    analogWrite(LED_WARM, 0);
  }

}
