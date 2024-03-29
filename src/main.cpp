/*  
horizontal middle one pins
tilt bottom set of pins,
vertical is the top set of pins
*/

#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Servo.h>
#include "LittleFS.h"
#include <Wire.h>
#include <ArduinoJson.h>

#include <MPU6050_6Axis_MotionApps20.h>

DynamicJsonDocument doc(200);

#define XSTR(x) #x
#define STR(x) XSTR(x)

#define pinS0 D5 
#define pinS1 D0
#define pinZ A0

int analog0;
int analog1;
int analog2;
int analog3;

//sets wifi up from env
const char* ssid = STR(SSID);
const char* password = STR(PASS);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncEventSource events("/events");

String horizontalString = String(0);
String verticalString = String(180);
String tiltString = String(0);

int horizontalCalib = -180;
int verticalCalib = 19;
int tiltCalib = 0;

Servo horizontal, vertical, tilt;

int joystick;
int joystickSwitch = D4;
int joystickState = 1;
bool joyStickMode = false;

MPU6050 mpu;
float hcorrection,vcorrection,tcorrection;
int lnum = 0 ;
bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t initStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;
VectorInt16 aa;
VectorInt16 aaReal;
VectorInt16 aaWorld;
VectorFloat gravity;
float euler[3];
float hvt[3];
volatile bool mpuInterrupt = false;

IRAM_ATTR void dmpDataReady(){
  mpuInterrupt = true;
}

float currentTime, previousTime;
float joystickActive;

enum State
{
  Idle,
  HorizontalUpdate,
  VerticalUpdate,
  TiltUpdate,
};

State gimbalState = Idle;
State gimbalPreviousState = Idle;

State socketState = Idle;
State socketPreviousState = Idle;

enum Modes
{
  unsetMode,
  socketMode,
  joystickMode,
  gyroMode
};

Modes controlMode = unsetMode;

void changeState(State changeTo, bool socket){
  if(socket){
    socketPreviousState = socketState;
    socketState = changeTo;
  }else{
    gimbalPreviousState = gimbalState;
    gimbalState = changeTo;
  }
}

int analogReader(uint8_t write1,uint8_t write2,uint8_t write3){
    digitalWrite(pinS0,write1); 
    digitalWrite(pinS1,write2); 
    //digitalWrite(pinS2,write3); 
    return analogRead(pinZ);
}


void updateServos(bool socket){
  // get which state needs updating
  State updateState = socket ? socketState : gimbalState;

  switch(updateState){
    case 1: horizontal.write(horizontalString.toInt()); break;
    case 2: vertical.write(verticalString.toInt()); break;
    case 3: tilt.write(tiltString.toInt()); break;
    default: break;
  }

  changeState(Idle, socket);
}

void onSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {

        const uint8_t size = JSON_OBJECT_SIZE(11);
        StaticJsonDocument<size> message;
        DeserializationError err = deserializeJson(message, data);
        JsonObject obj = message.as<JsonObject>();

        if (err) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(err.c_str());
            return;
        }else{
          Serial.println("incoming event");
          Serial.println(obj["event"].as<String>());
          if(obj["event"].as<String>() == "horizontal"){
            horizontalString = obj["value"].as<String>();
            controlMode = socketMode;
            changeState(HorizontalUpdate, false); 
            updateServos(false);
          }else if(obj["event"].as<String>() == "vertical"){
            verticalString = obj["value"].as<String>();
            controlMode = socketMode;
            changeState(VerticalUpdate, false); 
            updateServos(false);
          }else if(obj["event"].as<String>() == "tilt"){
            tiltString = obj["value"].as<String>();
            controlMode = socketMode;
            changeState(TiltUpdate, false); 
            updateServos(false);
          }else if(obj["event"].as<String>() == "mode"){
            switch(obj["value"].as<int>()){
              case 0: 
                controlMode = unsetMode;
                break;
              case 1: 
                controlMode = socketMode;
                break;
              case 2: 
                controlMode = joystickMode;
                break;
              case 3: 
                controlMode = gyroMode;
                break;
              default: 
                controlMode = unsetMode;
                break;
            }

          }else if(obj["event"].as<String>() == "tiltcalib"){
            tiltCalib = obj["value"].as<int>();
          }else if(obj["event"].as<String>() == "horizontalcalib"){
            horizontalCalib = obj["value"].as<int>();
          }else if(obj["event"].as<String>() == "verticalcalib"){
            verticalCalib = obj["value"].as<int>();
          }

        }
    }
}


void onSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            onSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void updateClients() {
  String outMsg = "";
  const uint8_t size = JSON_OBJECT_SIZE(11);
  StaticJsonDocument<size> message;
  message["event"] = "horizontal";
  message["value"] = horizontalString;

  serializeJson(message, outMsg);
  ws.textAll(outMsg.c_str());
  changeState(Idle, false);

}

String processor(const String& var){
  if(var == "HORIZONTAL"){
    return horizontalString;
  }else if(var == "VERTICAL"){
    return verticalString;
  }else if(var == "TILT"){
    return tiltString;
  }else if(var == "MODE"){
    switch(controlMode){
      case 0: return "Unset Mode"; break;
      case 1: return "Web Mode"; break;
      case 2: return "Joystick Mode"; break;
      case 3: return "Gyro Mode"; break;
    }
  }else if(var == "TILTCALIB"){
    return String(tiltCalib);
  }else if(var == "VERTICALCALIB"){
    return String(verticalCalib);
  }else if(var == "HORIZONTALCALIB"){
    return String(horizontalCalib);
  }

  return String();
}


void setup() {
  Serial.begin(38400);//115200);

  //Joystick setup
  pinMode(joystickSwitch, INPUT);
  digitalWrite(joystickSwitch, HIGH);

  pinMode(pinS0,OUTPUT);
  pinMode(pinS1,OUTPUT);
  pinMode(pinZ, INPUT);

  horizontal.attach(D7,500,2500);
  vertical.attach(D6,500,2500);
  tilt.attach(D8,500,2500);
  currentTime = millis();

  // Gyro Setup
  Wire.begin();
  mpu.initialize();
  pinMode( RX, INPUT);
  initStatus = mpu.dmpInitialize();
  mpu.setXGyroOffset(20);
  mpu.setYGyroOffset(-70);
  mpu.setZGyroOffset(30);
  mpu.setZAccelOffset(1551);//or 1688 for calibration

  if(initStatus == 0){
    Serial.println("initialized MPU");
    mpu.setDMPEnabled(true);

    attachInterrupt(digitalPinToInterrupt(RX),dmpDataReady,RISING);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  }else{
    Serial.println("Failed to the initialize MPU");
  }

  // Website setup
  LittleFS.begin();

  WiFi.mode(WIFI_AP_STA);
  WiFi.hostname("Gimbal");
  Serial.println("Connecting to wifi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // print website url into serial monitor
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: 'http://");
  Serial.print(WiFi.localIP());
  Serial.print("/' ");
  Serial.println("");

  ws.onEvent(onSocketEvent);
  server.addHandler(&ws);

  // loads in CSS and HTML
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });


  server.begin();
}



void loop(){

  // only sets timer for joystick if mode is unset.
  if(controlMode == unsetMode){
    joystickState = digitalRead(joystickSwitch);

    if(joystickState == 0){
      joyStickMode = true;
      joystickActive = millis();
      Serial.println("Joystick activated");
    }
  }
  
  if(joyStickMode || controlMode == joystickMode){
    currentTime = millis();
    if((currentTime - joystickActive) >= 15000 && controlMode != joystickMode){
      Serial.println("Joystick timer end");
      joyStickMode = false;
    }else{
      joystick = analogReader(HIGH,LOW,LOW);
      joystick = map (joystick, 0,1023,0,180);
      horizontalString = joystick;
      horizontal.write(horizontalString.toInt());
      changeState(HorizontalUpdate, false);
      updateServos(false);

      Serial.print("horizontal joystick value: ");
      Serial.print(horizontalString.toInt());
      Serial.println("");
      
      
      joystick = analogReader(LOW,LOW,LOW);
      joystick = map(joystick, 0,1023,0,180);
      verticalString = joystick;
      changeState(VerticalUpdate, false);
      updateServos(false);

      Serial.print("vertical joystick value: ");
      Serial.print(joystick);
      Serial.println("");
    }
    
  delay(100);
    
  }else{
    // checks for gyro updates if mode is unset or in gyro mode
    if((dmpReady && controlMode == gyroMode) || (dmpReady && controlMode == unsetMode)){

      while(!mpuInterrupt && fifoCount < packetSize){
        if(mpuInterrupt && fifoCount < packetSize){
          fifoCount = mpu.getFIFOCount();
        }
      }

      mpuInterrupt = false;
      mpuIntStatus = mpu.getIntStatus();
      fifoCount = mpu.getFIFOCount();

      if((mpuIntStatus & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) || fifoCount >= 1024){
        mpu.resetFIFO();
        fifoCount = mpu.getFIFOCount();
      }else if (mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)){
        while(fifoCount < packetSize){ 
          fifoCount = mpu.getFIFOCount();
        }
        mpu.getFIFOBytes(fifoBuffer,packetSize);
        fifoCount -= packetSize;
      }

      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(hvt, &q,&gravity);

      hvt[0] = hvt[0] * 180 / M_PI;
      hvt[2] = hvt[2] * 180 / M_PI;
      hvt[1] = hvt[1] * 180 / M_PI;


/* 
      #broken gyro calibration for vertical
      hvt[0] = (hvt[0] * 180 / M_PI) + horizontalCalib; // horizontal
      hvt[2] = (hvt[2] * 360 / M_PI) + verticalCalib; // vertical
      hvt[1] = (hvt[1] * 180 / M_PI) + tiltCalib;//- 40; // tilt
      */

      // first time initialization it is calibrating and sets horizontal to 0
      if(lnum <= 300){
        hcorrection = hvt[0];
        vcorrection = hvt[2];
        tcorrection = hvt[1];
        lnum++;
      }else{
        // setup correction.
        float correctedH = hcorrection - hvt[0] + horizontalCalib;
        float correctedV = vcorrection - hvt[2] + verticalCalib;
        float correctedT = tcorrection - hvt[1] + tiltCalib;

        horizontalString = map(correctedH, -90, 90, 0, 180);
        if( horizontalString.toInt() < 180 && horizontalString.toInt() > 0){
          changeState(HorizontalUpdate, false);
          updateServos(false);
        }

        verticalString = map(correctedV, -90, 90, 180, 0);
        if( verticalString.toInt() < 180 && verticalString.toInt() > 0){
          changeState(VerticalUpdate, false);
          updateServos(false);
        }

        tiltString = map(correctedT, -90, 90, 180, 0);
        if( tiltString.toInt() < 180 && tiltString.toInt() > 0){
          changeState(TiltUpdate, false);
          updateServos(false);
        }


        Serial.println("H: " + horizontalString + " V: " + verticalString + " T: " + tiltString);

      }

    }

    if(gimbalState != Idle){
      updateClients();
      updateServos(false);
    }

    if(socketState != Idle){
      updateServos(true);
    }


    delay(10);
  }
  
  ws.cleanupClients();
}