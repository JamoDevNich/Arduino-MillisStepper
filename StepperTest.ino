#include "MillisStepper.h"

MillisStepper motor1 = MillisStepper(22, 23, 24, 25);

void setup()
{  
   motor1.setFullRevSteps(210);
   motor1.setActive(true); // this can be put in either the setup or the loop
}

void loop()
{
  motor1.setRPM(70);
  motor1.doRun();
}
