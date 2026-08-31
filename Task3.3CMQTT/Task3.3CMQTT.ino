#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>

char ssid[] = "vivo";
char pass[] = "tetrabond";

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.emqx.io";
const int port = 1883;

// LEDs
const int led1 = 6;
const int led2 = 7;

// Ultrasonic sensor
const int trigPin = 2;
const int echoPin = 3;


// this runs a perticular thing when a message is received

void onMessageReceived(int messageSize) {

  String topic = mqttClient.messageTopic();

  Serial.print("MQTT message received on: ");
  Serial.println(topic);

  Serial.print("Message: ");

  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
  }

  Serial.println();


  // WAVE makes LEDs go ON
  if (topic == "ES/Wave") {

    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);

    Serial.println("ES/Wave received -> LEDs ON");
  }


  // PAT makes LEDs go OFF
  if (topic == "ES/Pat") {

    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);

    Serial.println("ES/Pat received -> LEDs OFF");
  }
}


// this function gets distance from the ultrasonic sensor

float getDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.0343 / 2;
}


void publishWave() {

  Serial.println("WAVE DETECTED");

  mqttClient.beginMessage("ES/Wave");
  mqttClient.print("Jasmeet");
  mqttClient.endMessage();

  Serial.println("Published to ES/Wave");
}

void publishPat() {

  Serial.println("PAT DETECTED");

  mqttClient.beginMessage("ES/Pat");
  mqttClient.print("Jasmeet");
  mqttClient.endMessage();

  Serial.println("Published to ES/Pat");
}

void setup() {

  Serial.begin(9600);

  while (!Serial) {
    ;
  }


  // LED setup
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);


  // Ultrasonic setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  // wifi

  Serial.println("Connecting to Wi-Fi...");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {

    Serial.println("Trying again...");
    delay(5000);
  }

  Serial.println("Wi-Fi connected!");


  // mqtt

  mqttClient.setId("Nano33IoT-Jasmeet");

  mqttClient.onMessage(onMessageReceived);

  Serial.println("Connecting to MQTT...");

  if (!mqttClient.connect(broker, port)) {

    Serial.print("MQTT connection failed. Error = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("MQTT connected!");


  // Subscribe to topics
  mqttClient.subscribe("ES/Wave");
  mqttClient.subscribe("ES/Pat");

  Serial.println("Subscribed to ES/Wave");
  Serial.println("Subscribed to ES/Pat");

  Serial.println("System ready!");
}

void loop() {

  // Keeps MQTT connection active
  mqttClient.poll();


  // Reads ultrasonic sensor
  float distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");


  // detects PAT
  if (distance > 0 && distance <= 10) {

    publishPat();

    delay(2000);
  }


  // detects WAVE
  else if (distance > 20 && distance <= 50) {

    publishWave();

    delay(2000);
  }


  delay(200);
}