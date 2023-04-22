#include <Arduino.h>
#include <Servo.h> 

Servo horizontal, vertical, tilt;
void setup() 
{ 
  Serial.begin(115200);
  vertical.attach(D6, 500,2050);  // attaches the servo on pin 11 to the middle object
  horizontal.attach(D7);
  tilt.attach(D8);
} 
 
void loop() 
{ 
  vertical.write(0);
  horizontal.write(0);
  //middle.write(20);
  delay(1000); 
  horizontal.write(180);
  vertical.write(180);
  delay(3000); // doesn't constantly update the servos which can fry them
} 
