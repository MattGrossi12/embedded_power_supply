#include <Stepper.h>

#define in1 8
#define in2 9
#define in3 10
#define in4 11


const int stepsPerRevolution = 200;

Stepper myStepper (stepsPerRevolution, in1, in2, in3, in4);

void setup() {
  // put your setup code here, to run once:

myStepper.setSpeed(240);

}

void loop() {
  // put your main code here, to run repeatedly:

myStepper.step(stepsPerRevolution);



}
