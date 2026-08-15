#include "thingProperties.h"
// defined the pin for porch and hallway
const int Pin_Porch = 5;
const int Pin_Hallway = 6;

// variables for the timer
long porchTime = 0;
long hallwayTime = 0;

// used to check if timer started or not
bool porchTimerStarted = false;
bool hallwayTimerStarted = false;

// stores if switch was on or off
bool lastSwitchState = false;

void setup() 
{
  // this initializes serial and wait for port to open:
  Serial.begin(9600);
  // this delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // LED pins
  pinMode(Pin_Porch, OUTPUT);
  pinMode(Pin_Hallway, OUTPUT);

  // LED are off at start
  digitalWrite(Pin_Porch, LOW);
  digitalWrite(Pin_Hallway, LOW);


  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() 
{
  ArduinoCloud.update();

  checkSwitch();
  checkPorchTimer();
  checkHallwayTimer();
}

//function to check if switch pressed and take action
void checkSwitch() 
{
  // detects if switch is pressed
  if (Led_Switch == true && lastSwitchState == false) 
  {
    Serial.println("Switch pressed!");

    // turns both lights on
    digitalWrite(Pin_Porch, HIGH);
    digitalWrite(Pin_Hallway, HIGH);

    Led_porch = true;
    Led_Hallway = true;

    // this starts the timer
    porchTime = millis();
    hallwayTime = millis();

    porchTimerStarted = true;
    hallwayTimerStarted = true;
  }

  // this saves last switch state
  lastSwitchState = Led_Switch;
}

// this function turns of porch light after 30sec
void checkPorchTimer() 
{
  // porch light on for 30 sec
  if (porchTimerStarted && millis() - porchTime >= 30000) 
  {
    digitalWrite(Pin_Porch, LOW);
    Led_porch = false;
    porchTimerStarted = false;
    //prints porch off in serial monitor
    Serial.println("Porch OFF");
  }

// this function turns of hallway light after 30sec
  void checkHallwayTimer() 
  {
    // hallway light on for 60 sec
    if (hallwayTimerStarted && millis() - hallwayTime >= 60000) 
    {
      digitalWrite(Pin_Hallway, LOW);
      Led_Hallway = false;
      hallwayTimerStarted = false;
      //prints hallway off in serial monitor
      Serial.println("Hallway OFF");
    }
  }
