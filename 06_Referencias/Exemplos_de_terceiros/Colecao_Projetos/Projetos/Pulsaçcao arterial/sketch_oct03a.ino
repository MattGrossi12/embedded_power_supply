
int ledPin = 8;
int sensorPin = 0;

double alpha = 0.75;
int period = 500;
double change = 0.0;

void setup() {
  
  // put your setup code here, to run once:
pinMode (ledPin, OUTPUT);
Serial.begin (9600);
}

void loop() {
  
 
static double oldValue = 0;
static double oldChange = 0;
int rawValue =
analogRead (sensorPin);
double value = alpha * oldValue
+ (1 - alpha) * rawValue;

//digitalWrite (ledPin, (change <
//0.0 && oldChanhe > 0.0));

//oldValue = value;
//oldChange = change;
//delay (period);

Serial.print (rawValue);
Serial.print(",");
Serial.println (value);

oldValue = value;
delay (period);

}
