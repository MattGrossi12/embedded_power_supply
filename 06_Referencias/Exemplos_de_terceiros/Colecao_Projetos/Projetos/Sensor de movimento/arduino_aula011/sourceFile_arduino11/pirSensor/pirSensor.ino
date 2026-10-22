#include <Servo.h>

#define flag   13
#define servo  12
#define pir     2


void readSensor();

unsigned long sensorIn = 0;

Servo actuator;

void setup()
{
   pinMode(servo, OUTPUT);
   pinMode(flag,  OUTPUT);
   pinMode(pir, INPUT);
   
   Serial.begin(9600);
   
   actuator.attach(12);
   
   actuator.write(90);

}

void loop()
{
   readSensor();
   
   if(sensorIn > 1E6 && sensorIn < 3E6)
   {
     digitalWrite(flag,HIGH);
     actuator.write(180);
   }
   else
   {
     digitalWrite(flag,LOW);
     actuator.write(0);
   }
// delay(100);
}

void readSensor()
{

   sensorIn = pulseIn(pir, HIGH);
   Serial.println(sensorIn);
   delay(1000);
}


