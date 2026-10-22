
//https://www.youtube.com/watch?v=gVl4B7NDnQY&t=7s

#include<LiquidCrystal.h>
LiquidCrystal lcd(6,7,8,9,10,11);

float value=0;
float rev=0;
int rpm;
int oldtime=0;
int time;



void isr() //interrupt service routine
{
rev++;
}

void setup()
{
lcd.begin(16,2);                //initialize LCD
attachInterrupt(0,isr,RISING);  //attaching the interrupt

pinMode (2, INPUT);

}

void loop()
{
delay(1000);
detachInterrupt(0);           //detaches the interrupt
time=millis()-oldtime;        //finds the time 
rpm=(rev/time)*60000;         //calculates rpm
oldtime=millis();             //saves the current time
rev=0;
lcd.clear();
lcd.setCursor(0,0);
lcd.print("___TACHOMETER___");
lcd.setCursor(0,1);
lcd.print(     rpm);
lcd.print(" RPM");
lcd.print("   ");
attachInterrupt(0,isr,RISING);
delay(1000);
}
