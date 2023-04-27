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
