#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Servo.h>
#include "LittleFS.h"

#define XSTR(x) #x
#define STR(x) XSTR(x)

//sets wifi up from env
const char* ssid = STR(SSID);
const char* password = STR(PASS);

AsyncWebServer server(80);

String horizontalString = String(20);

Servo horizontal, vertical, tilt;

int joystick;

int joystickX = A0;
int joystickY = D1;
int joystickSwitch = D5;
int joystickState = 0;

String processor(const String& var){
  Serial.println(var);
  // retrieve state value 
  //TODO CHANGE TO HORIZONTAL
  if(var == "STATE"){
    Serial.print(horizontalString);
    return horizontalString;
  }
  return String();
}

void setup() {
  Serial.begin(115200);
  pinMode(joystickSwitch, INPUT_PULLUP);
  horizontal.attach(D6,500,2050);
  vertical.attach(D7);
  tilt.attach(D8);


  LittleFS.begin();
/*
  TODO FIND AN ALTERNATIVE FOR ERROR REPORTING IN LITTLEFS
  if(!LittleFS.begin(true)){
    Serial.println("LittleFs Failed to mount");
    return;
  }*/



  WiFi.hostname("Gimbal");
  Serial.println("Connecting to wifi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: 'http://");
  Serial.print(WiFi.localIP());
  Serial.print("/' ");
  Serial.println("");

  // Load web sever pages and css
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });

  //catch web server endpoint for horizontal servo
  server.on("/horizontal", HTTP_GET, [](AsyncWebServerRequest *request){
    // check for slider value
    if(request->hasParam("value")){
      AsyncWebParameter* p = request->getParam("value");
      Serial.println(p->value());
      horizontal.write(p->value().toInt());
      horizontalString = p->value();
    }
    // update STATE
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.begin();
}

void loop(){
/*
  TODO ONLY TURN ON WHEN BUTTON IS PRESSED OTHERWISE USE WEBSITE VERSION MAYBE
  joystick = analogRead(joystickX);
  joystick = map (joystick, 0,1023,0,180);
  // tell servo.
  horizontal.write(joystick);

  joystick = analogRead(joystickY);
  joystick = map(joystick, 0,1023,0,180);
  vertical.write(joystick);

  joystickState = digitalRead(joystickSwitch);

  delay(100);
  */
}