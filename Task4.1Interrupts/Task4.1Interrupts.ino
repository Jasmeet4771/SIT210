#include <Wire.h>
#include <BH1750.h>

// definig pins
const int PIR_PIN = 2;
const int BUTTON_PIN = 3;

const int LED1_PIN = 6;
const int LED2_PIN = 7;

// Light sensor
BH1750 lightMeter;

//darkness threshold
const float DARK_THRESHOLD = 50.0;

// Timers to turn light off automaticly 
const unsigned long LED1_TIME = 30000;  
const unsigned long LED2_TIME = 60000;  

unsigned long led1StartTime = 0;
unsigned long led2StartTime = 0;

//  these are states to tell which was triggered 
volatile bool pirTriggered = false;
volatile bool buttonTriggered = false;

bool lightsManuallyOff = false;

bool led1Active = false;
bool led2Active = false;

// Button debounce
volatile unsigned long lastButtonInterrupt = 0;
const unsigned long DEBOUNCE_TIME = 200;

//  Interrupts 

void pirInterrupt() {
  pirTriggered = true;
}

void buttonInterrupt() {
  unsigned long currentTime = millis();

  if (currentTime - lastButtonInterrupt > DEBOUNCE_TIME) {
    buttonTriggered = true;
    lastButtonInterrupt = currentTime;
  }
}
 

void setup() {
  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  Wire.begin();

  if (lightMeter.begin()) {
    Serial.println("BH1750 started successfully.");
  } else {
    Serial.println("Error: BH1750 not detected.");
  }

  attachInterrupt(digitalPinToInterrupt(PIR_PIN),
                  pirInterrupt,
                  RISING);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),
                  buttonInterrupt,
                  FALLING);
}


void loop() {

  // Reads light levels
  float lux = lightMeter.readLightLevel();

  bool isDark = lux < DARK_THRESHOLD;

  //  pir event

  if (pirTriggered) {

    noInterrupts();
    pirTriggered = false;
    interrupts();

    Serial.println();
    Serial.println("PIR: Motion detected.");

    if (isDark) {

      Serial.print("It is dark. Light level: ");
      Serial.print(lux);
      Serial.println(" lux.");

      lightsManuallyOff = false;

      turnLightsOn();

      Serial.println("Lights activated by PIR.");
    }
    else {

      Serial.print("It is not dark. Light level: ");
      Serial.print(lux);
      Serial.println(" lux.");

      Serial.println("Lights not activated because it is bright.");
    }
  }

  //  button event 

  if (buttonTriggered) {

    noInterrupts();
    buttonTriggered = false;
    interrupts();

    Serial.println();
    Serial.println("BUTTON: Push button pressed.");


    if (led1Active || led2Active) {

      turnLightsOff();

      lightsManuallyOff = true;

      Serial.println("Lights manually turned OFF.");

    } else {

      lightsManuallyOff = false;

      turnLightsOn();

      Serial.println("Lights manually turned ON.");
    }
  }


  unsigned long currentTime = millis();

  // LED 1 for 30 seconds
  if (led1Active &&
      currentTime - led1StartTime >= LED1_TIME) {

    digitalWrite(LED1_PIN, LOW);
    led1Active = false;

    Serial.println("LED1: 30 seconds completed. LED1 OFF.");
  }

  // LED 2 for 60 seconds
  if (led2Active &&
      currentTime - led2StartTime >= LED2_TIME) {

    digitalWrite(LED2_PIN, LOW);
    led2Active = false;

    Serial.println("LED2: 60 seconds completed. LED2 OFF.");
  }

  delay(50);
}

//  functions for light

void turnLightsOn() {

  unsigned long currentTime = millis();

  // Turns LED1 ON
  digitalWrite(LED1_PIN, HIGH);
  led1Active = true;
  led1StartTime = currentTime;

  // Turns LED2 ON
  digitalWrite(LED2_PIN, HIGH);
  led2Active = true;
  led2StartTime = currentTime;

  Serial.println("LED1 ON for 30 seconds.");
  Serial.println("LED2 ON for 60 seconds.");
}

void turnLightsOff() {

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  led1Active = false;
  led2Active = false;
}