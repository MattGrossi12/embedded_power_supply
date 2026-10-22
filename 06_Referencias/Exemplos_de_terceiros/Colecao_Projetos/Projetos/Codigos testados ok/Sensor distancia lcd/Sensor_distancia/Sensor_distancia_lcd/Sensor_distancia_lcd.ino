
/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
*
* Crated by Dejan Nedelkovski,
* http://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
*
*/

#include <LiquidCrystal.h> // includes the LiquidCrystal Library

LiquidCrystal lcd(8,9, 4, 5, 6, 7); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)

const int trigPin = 11;
const int echoPin = 12;
long duration;
float distanceCm;
//int distanceInch;

void setup() {
lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
}

void loop() {
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distanceCm= duration*0.034/2;
//distanceInch = duration*0.0133/2;
lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
lcd.print("Distancia: "); // Prints string "Distance" on the LCD
lcd.print(distanceCm); // Prints the distance value from the sensor
lcd.print(" cm");
delay(100);
lcd.setCursor(0,1);
//lcd.print("Distancia: ");
//lcd.print(distanceInch);
//lcd.print(" inch");
//delay(10);
}
