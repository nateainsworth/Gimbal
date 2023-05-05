# Robotic Gimbal
The Project makes use of two ESP micro controllers. One handles the camera and hosts it's video feed on the local network. The other controls the 3 servos taking readings and adjusting the servos from the gyrometer, accelerometer, joystick and sliders on a website hosted on the local network by the micro controller.

## **Table of Contents**
- [**Setup and Runing the Robot**](#setup-and-running-the-robot)
    - [**Setting up the Camera**](#setting-up-the-camera)
    - [**Setting up the Gimbal**](#setting-up-the-gimbal)
    - [**Running the Gimbal**](#running-the-gimbal)
- [**Different Modes**](#different-modes)
    - [**Unset Mode**](#unset-mode)
    - [**Web Mode**](#web-mode)
    - [**Joystick Mode**](#joystick-mode)
    - [**Gyro Mode**](#gyro-mode)
- [**References**](#references)


# Setup and Running the Robot

## Setting up the Camera
1. Open within your desired IDE VS Code with the platformIO extension is recommended.
2. Add / edit an env file containing 'SSID' and 'PASS' with the local network id and password.
3. Build and upload the code (Make sure to plug the camera in via USB).
4. Power the Camera from a PC to find out the URL of the website found at the start of the serial monitor.
5. Restart the camera either plugged in or via 9v battery.
6. Go to the URL provided by the serial monitor. 

## Setting up the Gimbal
1. Open within your desired IDE VS Code with the platformIO extension is recommended.
2. Add / edit an env file containing 'SSID' and 'PASS' with the local network id and password.
2. Build and upload the code (Make sure the Gimbal is plugged in via USB).
3. Power the gimbal via USB to find the URL of the hosted website found at the start of the serial monitor.
4. Restart the gimbal either plugged in or via a 9v battery.
5. Go to the URL provided by the serial monitor
6. You may need to update the Gyro offsets.

## Running the Gimbal
1. The gimbal will start in an unset mode while it will appear to use the gyro holding down the joystick will change control to the joystick for a short time before it times out.
2. On the hosted website you also have the options to change the horizontal, vertical and tilt of the gyro, this will change it to Web controlled mode.
3. The website also features buttons for changing back to unset mode or for switching fully into joystick or gyro mode.

<br />


# Different Modes
There are 4 different modes which can be used to control the direction of the camera on the gimbal.

## Unset Mode
Will default to gyro control servos, but can be controlled by holding the joystick down activating a timer allowing control of the joystick for a short period of time.

## Web Mode
Turns off control for both the gyro and joystick. Allowing the gimbal to be fully controlled using the sliders on the website.

## Joystick Mode
Enables control of both the horizontal and vertical axes of the gimbal although doesn't enable control of tilt

## Gyro Mode
Controls the gimbal from the gyro attempting to keep the gimbal as stable as it can while moving around.


# References

Geißler, F. (2020, June 10). Access of outer environment variable in Platformio. Stack Overflow. Retrieved from https://stackoverflow.com/questions/62314497/access-of-outer-environment-variable-in-platformio 

randomnerdtutorials. (2023, March 11). Getting started with the freenove ESP32-wrover cam board. Random Nerd Tutorials. Retrieved from https://randomnerdtutorials.com/getting-started-freenove-esp32-wrover-cam/ 

Maker 101. (2016, December 15). Multiple servo motor control with joystick and Arduino. YouTube. Retrieved from https://www.youtube.com/watch?v=kA_pbMR6jVs 

RandomNerdTutorials, S. (2019, April 2). ESP32 servo motor web server with Arduino Ide. Random Nerd Tutorials. Retrieved from https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/ 

RandomNerdTutorials. (2019, August 22). ESP32 web server using SPIFFS (SPI flash file system). Random Nerd Tutorials. Retrieved from https://randomnerdtutorials.com/esp32-web-server-spiffs-spi-flash-file-system/ 

RandomNerdTutorials. (2020, December 23). Esp8266 vs Code Platformio: Upload files to filesystem littlefs. Random Nerd Tutorials. Retrieved from https://randomnerdtutorials.com/esp8266-nodemcu-vs-code-platformio-littlefs/ 

Solarduino. (2020, June 15). Add or increase analog pins for NODEMCU or Arduino using 16 channel analog multiplexer module. YouTube. Retrieved from https://www.youtube.com/watch?v=We_kBjiUkXU&amp;ab_channel=Solarduino 

Gonzaloee, & Amorphous. (2019, July 7). Error ISR not in Iram! Blynk Community. Retrieved April 26, 2023, from https://community.blynk.cc/t/error-isr-not-in-iram/37426/16 

TheoL. (2021, October 9). My wemos D1 mini adventures - interrupts on pins D3 and D4. MySensors Forum. Retrieved from https://forum.mysensors.org/topic/11822/my-wemos-d1-mini-adventures-interrupts-on-pins-d3-and-d4 

Rowberg, J. (2023, February). jrowberg/I2Cdevlib-MPU6050. Platformio registry. Retrieved from https://registry.platformio.org/libraries/jrowberg/I2Cdevlib-MPU6050 