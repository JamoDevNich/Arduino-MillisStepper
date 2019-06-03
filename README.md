# Arduino-MillisStepper
A non-blocking controller for running multiple L298N/L9110S dual h-bridge drivers with a single arduino

## Getting Started
1. Copy and include `MillisStepper.h` in your project
2. Declare your motor `motor1(pin1, pin2, pin3, pin4)`
3. In the setup, set your motor's full steps `motor1.setFullRevSteps(int)`
4. Enable the motor using `motor1.setActive(true)`
5. Set the speed using `motor1.setRPM(int)`
6. To run the motor, place `motor1.doRun()` in the loop

## Features
- Acceleration and Deceleration
- Clockwise and anticlockwise motor rotation

## Please Note
- This implementation does not use interrupts (yet), so motors may operate louder and warmer than usual.
- Tested on a MEGA 2650 with 5 stepper motors
- Individual steps cannot be done
- Acceleration and deceleration is enabled by default - to bypass this feature, set the motor speed then enable the motor.
- Maximum motor speed is limited by your arduino's operating speed

