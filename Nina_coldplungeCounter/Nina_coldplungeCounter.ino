// Includes
#include <EEPROM.h>

// Debug parameters
#define DEBUG_SERIAL

// Pin settings
#define BTN_COUNT_UP    (10)
#define LED_NUM_1     	(2)
#define LED_NUM_2     	(3)
#define LED_NUM_4     	(4)
#define LED_NUM_8     	(5)
#define LED_NUM_16    	(6)
#define LED_NUM_32    	(7)
#define LED_NUM_64    	(8)
#define LED_NUM_128   	(9)
#define LED_CNT       	(8)

const uint8_t leds[][2] = {
  {LED_NUM_1,   0x01},  
  {LED_NUM_2,   0x02},  
  {LED_NUM_4,   0x04},  
  {LED_NUM_8,   0x08},  
  {LED_NUM_16,  0x10}, 
  {LED_NUM_32,  0x20}, 
  {LED_NUM_64,  0x40}, 
  {LED_NUM_128, 0x80}
};

// Timing constants
#define DEBOUNCE_BTN_us           (50000)
#define COUNT_UP_START_TIME_ms    (50)
#define COUNT_UP_OFF_DELAY        (20)
#define COUNT_UP_ON_DELAY         (20)

// Memory constants
#define VADDR_CP_COUNTER          (0)

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < LED_CNT; i += 1){
    pinMode(leds[i][0], OUTPUT);
    digitalWrite(i, LOW);
  }
  pinMode(BTN_COUNT_UP, INPUT_PULLUP);
#ifdef DEBUG_SERIAL
  Serial.begin(9600);
#endif
}

void visualReset(){
  for (uint8_t i = 0; i < LED_CNT; i += 1){
    digitalWrite(leds[i][0], HIGH);
    delay(COUNT_UP_ON_DELAY);
  }
  for (uint8_t i = 0; i < LED_CNT; i += 1){
    digitalWrite(leds[i][0], LOW);
    delay(COUNT_UP_OFF_DELAY);
  }
}

void showCountOnLeds(uint8_t cnt){
  uint32_t sleepTime = COUNT_UP_START_TIME_ms;
  visualReset();
  for (uint8_t i = 0; i < LED_CNT; i += 1){
    uint8_t setToState = cnt & leds[i][1] ? HIGH : LOW;
    digitalWrite(leds[i][0], setToState);
    if (setToState == HIGH){ 
      // we only sleep if the LED is set to HIGH. Looks better
      delay(sleepTime);
    }
  }
}

void loop() {
  long lastPressed = micros();
  uint8_t lastButtonState = HIGH;
  uint8_t coldPlungeCount = 0;
  EEPROM.get(VADDR_CP_COUNTER, coldPlungeCount);
  showCountOnLeds(coldPlungeCount);
  while(1){
    uint8_t buttonState = digitalRead(BTN_COUNT_UP);
    if (buttonState == LOW && lastButtonState == HIGH) {
        if (micros() - lastPressed > DEBOUNCE_BTN_us) {
            lastPressed = micros();
            coldPlungeCount += 1;
            EEPROM.update(VADDR_CP_COUNTER, coldPlungeCount);
#ifdef DEBUG_SERIAL
            Serial.print("Count: ");
            Serial.println(coldPlungeCount);
#endif
            showCountOnLeds(coldPlungeCount);
        }
    }
    lastButtonState = buttonState;
  }
}


















