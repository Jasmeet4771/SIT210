#include "thingProperties.h"
#include <WiFiNINA.h>

//defining led pins
const int LIVING_ROOM_LED = 5;
const int BATHROOM_LED = 3;
const int CLOSET_LED = 4;


//wifi passwords and id
char ssid[] = "Airtel_aman_8300";
char pass[] = "Air@19106";

//web server
WiFiServer server(80);


// this fuction checks the room and then turns its light on
void setRoom(String room, bool state) {

  // checks for living room
  if (room == "living room") {

    livingRoom = state;

    digitalWrite(
      LIVING_ROOM_LED,
      state ? HIGH : LOW
    );

    Serial.print("Living Room: ");
    Serial.println(state ? "ON" : "OFF");
  }

  //checks for bathroom
  else if (room == "bathroom") {

    bathroom = state;

    digitalWrite(
      BATHROOM_LED,
      state ? HIGH : LOW
    );

    Serial.print("Bathroom: ");
    Serial.println(state ? "ON" : "OFF");
  }

  // checks for closet
  else if (room == "closet") {

    closet = state;

    digitalWrite(
      CLOSET_LED,
      state ? HIGH : LOW
    );

    Serial.print("Closet: ");
    Serial.println(state ? "ON" : "OFF");
  }
}


// setup code
void setup() {

  Serial.begin(9600);

  delay(1500);


// led setup
  pinMode(LIVING_ROOM_LED, OUTPUT);
  pinMode(BATHROOM_LED, OUTPUT);
  pinMode(CLOSET_LED, OUTPUT);


// all leds are off initially

  digitalWrite(LIVING_ROOM_LED, LOW);
  digitalWrite(BATHROOM_LED, LOW);
  digitalWrite(CLOSET_LED, LOW);


// arduino cloud part
  initProperties();

  ArduinoCloud.begin(
    ArduinoIoTPreferredConnection
  );

  setDebugMessageLevel(2);

  ArduinoCloud.printDebugInfo();


  // for wifi 
  Serial.println();
  Serial.println("Connecting to Wi-Fi...");

  // this tries to connect
  WiFi.begin(ssid, pass);

  // until wifi is not connected it prints "."
  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");
  }


  Serial.println();

  Serial.println("Wi-Fi connected");


  // this shows the ip of my arduino
  Serial.print("Arduino IP Address: ");

  Serial.println(WiFi.localIP());


  //starts teh web server
  server.begin();

  Serial.println("Web server started");
}


//loop code

void loop() {

  // this keeps Arduino Cloud running

  ArduinoCloud.update();


  // Check for browser connection

  WiFiClient client = server.available();


  if (client) {

    Serial.println("New web client");


    String request = "";

    unsigned long timeout = millis();


    // checks for timeout
    while (
      client.connected() &&
      millis() - timeout < 1000
    ) {

      if (client.available()) {

        char c = client.read();

        request += c;


        // HTTP request finished

        if (request.endsWith("\r\n\r\n")) {

          break;
        }
      }
    }


    Serial.println(request);


    //living room turing on
    if (request.indexOf("GET /toggle?room=living&state=on") >= 0) 
    {
      setRoom("living room", true);
    }

    // living room turing off
    else if (request.indexOf("GET /toggle?room=living&state=off") >= 0) 
    {
      setRoom("living room", false);
    }


    // for turing on bathroom led
    else if (request.indexOf("GET /toggle?room=bathroom&state=on") >= 0) 
    {
      setRoom("bathroom", true);
    }

    // for turning off bathroom light
    else if (request.indexOf("GET /toggle?room=bathroom&state=off") >= 0) {
      setRoom("bathroom", false);
    }


    // for turing on closet light
    else if (request.indexOf("GET /toggle?room=closet&state=on") >= 0) {
      setRoom("closet", true);
    }

    // for turning off closet light
    else if (request.indexOf("GET /toggle?room=closet&state=off") >= 0) {
      setRoom("closet", false);
    }


    // sends web page
    client.println("HTTP/1.1 200 OK");

    client.println(
      "Content-Type: text/html"
    );

    client.println(
      "Connection: close"
    );

    client.println();


    // html part

    client.println("<!DOCTYPE html>");

    client.println("<html>");

    client.println("<head>");

    client.println(
      "<meta name='viewport' "
      "content='width=device-width, initial-scale=1'>"
    );

    client.println(
      "<title>Remote Light Control</title>"
    );


    // css part

    client.println("<style>");

    client.println(
      "body {"
      "font-family: Arial;"
      "background: white;"
      "margin: 30px;"
      "}"
    );


    client.println(
      ".container {"
      "width: 500px;"
      "margin: auto;"
      "}"
    );


    client.println(
      ".room {"
      "border: 1px solid #ccc;"
      "padding: 15px;"
      "margin: 10px 0;"
      "}"
    );


    client.println(
      "h1 {"
      "font-size: 24px;"
      "}"
    );


    client.println(
      "h2 {"
      "font-size: 18px;"
      "}"
    );


    client.println(
      "label {"
      "margin-right: 15px;"
      "}"
    );


    client.println("</style>");

    client.println("</head>");


    // body part

    client.println("<body>");

    client.println("<div class='container'>");


    client.println(
      "<h1>Remote Light Control</h1>"
    );


    client.println(
      "<p>Assisted Living Smart Lighting System</p>"
    );


    // buttons for living room

    client.println("<div class='room'>");

    client.println(
      "<h2>Living Room</h2>"
    );


    client.println(
      "<form action='/toggle' method='GET'>"
    );


    client.println(
      "<input type='hidden' "
      "name='room' value='living'>"
    );


    client.println(
      "<label>"
      "<input type='radio' "
      "name='state' value='on' "
      "onclick='this.form.submit()'> ON"
      "</label>"
    );


    client.println(
      "<label>"
      "<input type='radio' "
      "name='state' value='off' "
      "onclick='this.form.submit()'> OFF"
      "</label>"
    );


    client.println("</form>");

    client.println("</div>");


    // buttons for bathroom

    client.println("<div class='room'>");

    client.println(
      "<h2>Bathroom</h2>"
    );


    client.println(
      "<form action='/toggle' method='GET'>"
    );


    client.println(
      "<input type='hidden' "
      "name='room' value='bathroom'>"
    );


    client.println(
      "<label>"
      "<input type='radio' "
      "name='state' value='on' "
      "onclick='this.form.submit()'> ON"
      "</label>"
    );


    client.println(
      "<label>"
      "<input type='radio' "
      "name='state' value='off' "
      "onclick='this.form.submit()'> OFF"
      "</label>"
    );


    client.println("</form>");

    client.println("</div>");


    // buttons for closet
    client.println("<div class='room'>");

    client.println(
      "<h2>Closet</h2>"
    );


    client.println(
      "<form action='/toggle' method='GET'>"
    );


    client.println(
      "<input type='hidden' "
      "name='room' value='closet'>"
    );


    client.println(
      "<label>"
      "<input type='radio' "
      "name='state' value='on' "
      "onclick='this.form.submit()'> ON"
      "</label>"
    );


    client.println(
      "<label>"
      "<input type='radio' "
      "name='state' value='off' "
      "onclick='this.form.submit()'> OFF"
      "</label>"
    );


    client.println("</form>");

    client.println("</div>");


    client.println("</div>");

    client.println("</body>");

    client.println("</html>");


    
    delay(1);

    client.stop();

    Serial.println(
      "Client disconnected"
    );
  }
}


// arduino cloud callbacks 
void onBathroomChange() {

  digitalWrite(
    BATHROOM_LED,
    bathroom ? HIGH : LOW
  );
}


void onClosetChange() {

  digitalWrite(
    CLOSET_LED,
    closet ? HIGH : LOW
  );
}


void onLivingRoomChange() {

  digitalWrite(
    LIVING_ROOM_LED,
    livingRoom ? HIGH : LOW
  );
}