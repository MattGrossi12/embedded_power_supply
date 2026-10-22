
#include <Time.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define TIME_MSG_LEN 11 // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_REQUEST 7 // ASCII bell character requests a time sync message 

LiquidCrystal_I2C lcd(56,16,2);   //If 

long previousmillis=0;
int hr=0;
int minut=0;
int dia=0;
int mes=0;
int ano=0;
int count=0;
void setup() {

lcd.init();
lcd.backlight();

pinMode(2,INPUT);
pinMode(3,INPUT);
pinMode(4,INPUT);
pinMode(5,INPUT);
pinMode(6,INPUT);
pinMode(7,INPUT);
Serial.begin(9600);
Serial.println("Waiting for sync message");

lcd.print("Waiting");
lcd.setCursor(0,1);
lcd.print("for sync message");
setTime(0,0,0,1,1,0);
}
void loop(){ 

if(Serial.available() ) 
{
processSyncMessage();
}


if((digitalRead(7)==HIGH))     //Change button
{
count=count+1;
Serial.println("OK");
if(count>1)
{
count=0; 
}
}
if((digitalRead(3)==HIGH) && count==1)  //Hour button
{
hr=3600;
adjustTime(hr);
}
if((digitalRead(2)==HIGH) && count==1)   //Minute button
{
minut=60;
adjustTime(minut);
}
if((digitalRead(6)==HIGH) && count==1)   //Day Button
{
dia++;
if(dia+day()>31)
{
dia=1-day();
} 
}
if((digitalRead(5)==HIGH) && count==1)   //Month button
{
mes++;
if(mes+month()>12)
{
mes=1-month(); 
}
}
if((digitalRead(4)==HIGH) && count==1)    //Year Button
{
ano++;

}

if(timeStatus()!= timeNotSet) 
{
digitalWrite(13,timeStatus() == timeSet); // on if synced, off if needs refresh 
digitalClockDisplay(); 
}
delay(1000);

}
void digitalClockDisplay(){
// digital clock display of the time
lcd.clear();
lcd.print("Hora: ");
lcd.print(hour());
Serial.print(hour());
printDigits(minute());
printDigits(second());
lcd.setCursor(0,1);
Serial.print(" ");
Serial.print(dia+day());
lcd.print("Data: ");
lcd.print(dia+day());
lcd.print(".");
Serial.print(" ");
Serial.print(mes+month());
lcd.print(mes+month());
lcd.print(".");
Serial.print(" ");
Serial.print(ano+year());
lcd.print(ano+year()); 
Serial.println(); 
}
void printDigits(int digits){
// utility function for digital clock display: prints preceding colon and leading 0
Serial.print(":");
lcd.print(":");
if(digits < 10){
Serial.print('0');
lcd.print("0");
}
Serial.print(digits);
lcd.print(digits);
}
void processSyncMessage() {
// if time sync available from serial port, update time and return true
while(Serial.available() >= TIME_MSG_LEN ){ // time message consists of a header and ten ascii digits
char c = Serial.read() ; 
Serial.print(c); 
if( c == TIME_HEADER ) { 
time_t pctime = 0;
for(int i=0; i < TIME_MSG_LEN -1; i++){ 
c = Serial.read(); 
if( c >= '0' && c <= '9'){ 
pctime = (10 * pctime) + (c - '0') ; // convert digits to a number 
}
} 
setTime(pctime); // Sync Arduino clock to the time received on the serial port
} 
}
}
time_t requestSync()
{
Serial.write(TIME_REQUEST); 
return 0; // the time will be sent later in response to serial mesg
}
