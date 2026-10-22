#include <LiquidCrystal.h>
int val;
long last=0;
int stat=HIGH;
int stat2;
int contar=0;

int sens=1000; // this value indicates the limit reading between dark and light,
// it has to be tested as it may change acording on the
// distance the leds are placed.
int nPalas=1; // the number of blades of the propeller

int milisegundos=500; // the time it takes each reading
LiquidCrystal lcd(9, 8, 7, 6, 5, 4); // RS, E, D4, D5, D6, D7
void setup()
{
//Serial.begin(9600);
lcd.begin(16, 2);
lcd.clear();

pinMode(13,OUTPUT);
}

void loop()
{
val=analogRead(0);
if(val<sens)
stat=HIGH;
else
stat=LOW;
digitalWrite(13,stat); //as iR light is invisible for us, the led on pin 13
//indicate the state of the circuit.

if(stat2!=stat){ //counts when the state change, thats from (dark to light) or
//from (light to dark), remmember that IR light is invisible for us.
contar++;
stat2=stat;
}
if(millis()-last>=milisegundos){
int rps=((double)contar/nPalas)/2.0*1000.0/milisegundos;
int rpm=((double)contar/nPalas)/2.0*60000.0/(milisegundos);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("RPM = ");
lcd.print(rpm);

/*Serial.print("Contar - ");
Serial.println((contar/2.0));
Serial.print("RPS = ");
Serial.println(rps);
Serial.print("RPM = ");Serial.println(rpm);
Serial.print(" VAL ");Serial.println(val);*/
contar=0;
last=millis();
}
}
