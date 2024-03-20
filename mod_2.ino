/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-button-toggle-led
 */

#define BUTTON_PIN  33 // ESP32 pin GPIO18, which connected to button
#define LED_PIN     32 // ESP32 pin GPIO21, which connected to led

#define BUTTON_PIN_2  25 // ESP32 pin GPIO18, which connected to button
#define LED_PIN_2     12 // ESP32 pin GPIO21, which connected to led

#define POTENTIOMETER_PIN  13
#define LED_PIN_P          15
#define LED_PIN_P_2        17

#define BUTTON_SWITCH  2

// variables will change:
int led_state = LOW;    // the current state of LED
int button_state;       // the current state of button
int last_button_state;  // the previous state of button

int led_state_2 = LOW;    // the current state of LED
int button_state_2;       // the current state of button
int last_button_state_2;  // the previous state of button

int last_brightness;

int button_state_switch;
int last_button_state_switch;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

int led_pin_switch = LED_PIN_P;

void setup() {
  Serial.begin(9600);                // initialize serial

  pinMode(BUTTON_PIN, INPUT_PULLUP); // set ESP32 pin to input pull-up mode
  pinMode(BUTTON_PIN_2, INPUT_PULLUP); // set ESP32 pin to input pull-up mode
  pinMode(BUTTON_SWITCH, INPUT_PULLUP); // set ESP32 pin to input pull-up mode

  pinMode(LED_PIN, OUTPUT);          // set ESP32 pin to output mode
  pinMode(LED_PIN_2, OUTPUT);          // set ESP32 pin to output mode
  pinMode(LED_PIN_P, OUTPUT);

  button_state = digitalRead(BUTTON_PIN);
  button_state_2 = digitalRead(BUTTON_PIN_2);
  button_state_switch = digitalRead(BUTTON_SWITCH);
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);
  int reading_2 = digitalRead(BUTTON_PIN_2);
  int reading_switch = digitalRead(BUTTON_SWITCH);

  // If the switch changed, due to noise or pressing:
  if ((reading != last_button_state) || (reading_2 != last_button_state_2) || (reading_switch != last_button_state_switch)) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  // reads the input on analog pin A0 (value between 0 and 4095)
  int analogValue = analogRead(POTENTIOMETER_PIN);

  // scales it to brightness (value between 0 and 255)
  int brightness = map(analogValue, 0, 4095, 255, 0);

  analogWrite(led_pin_switch, brightness);

  if ((brightness - last_brightness) > 1 || (brightness - last_brightness) < -1 || (reading_switch != last_button_state_switch) || (reading != last_button_state) || (reading_2 != last_button_state_2)){
    Serial.print("PotValue:");
    Serial.println(brightness);
    last_brightness = brightness;
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != button_state) {
      button_state = reading;

      // only toggle the LED if the new button state is LOW
      if (button_state == LOW) {
        // Serial.println("The button is pressed");
        led_state = !led_state;
        if (led_state == LOW) {
          Serial.println("Cool light off");
        } else {
          Serial.println("Cool light on");
        }
        digitalWrite(LED_PIN, led_state);
      }
    }

    if (reading_2 != button_state_2) {
      button_state_2 = reading_2;

      // only toggle the LED if the new button state is LOW
      if (button_state_2 == LOW) {
        // Serial.println("The button 2 is pressed");
        led_state_2 = !led_state_2;
        if (led_state_2 == LOW) {
          Serial.println("Warm light off");
        } else {
          Serial.println("Warm light on");
        }
        digitalWrite(LED_PIN_2, led_state_2);
      }
    }

    if (reading_switch != button_state_switch) {
      button_state_switch = reading_switch;

      // only toggle the LED if the new button state is LOW
      if (button_state_switch == LOW) {
        // Serial.println("The button switch is pressed");
        if (led_pin_switch == LED_PIN_P_2) {
          led_pin_switch = LED_PIN_P;
          Serial.println("Dimming cool light");
        } else {
          led_pin_switch = LED_PIN_P_2;
          Serial.println("Dimming warm light");
        }
        
        // led_state_2 = !led_state_2;
        // digitalWrite(LED_PIN_2, led_state_2);
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  last_button_state = reading;
  last_button_state_2 = reading_2;
  last_button_state_switch = reading_switch;
  delay(10);
}
