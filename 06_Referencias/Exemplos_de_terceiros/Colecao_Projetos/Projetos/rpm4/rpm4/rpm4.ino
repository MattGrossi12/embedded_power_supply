/*
  Author: Chris @ PyroElectro.com
  Date: 8/26/2012
  Description:
  This project is meant to capture interrupt counts from an IR breakbeam circuit
  and display them as an RPM number on a 16x2 LCD module.
  
  Full Project Details:
  http://www.pyroelectro.com/tutorials/tachometer_rpm_arduino/
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(6,7,8,9,10,11);

volatile float time = 0;
volatile float time_last = 0;
volatile int rpm_array[5] = {0,0,0,0,0};

void setup()
{
  //Digital Pin 2 Set As An Interrupt
 attachInterrupt(0, fan_interrupt, FALLING);

  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Current RPM:");
}

void loop()
{
  int rpm = 0;
  
  while(1){    

     //Slow Down The LCD Display Updates
  delay(400);
  
  //Clear The Bottom Row
  lcd.setCursor(0, 1);
  lcd.print("                ");   
  
  //Update The Rpm Count
  lcd.setCursor(0, 1);
  lcd.print(rpm);   

  ////lcd.setCursor(4, 1);
  ////lcd.print(time);   

  //Update The RPM
  if(time > 0)
  {
    //5 Sample Moving Average To Smooth Out The Data
      rpm_array[0] = rpm_array[1];
      rpm_array[1] = rpm_array[2];
      rpm_array[2] = rpm_array[3];
      rpm_array[3] = rpm_array[4];
      rpm_array[4] = 60*(1000000/(time*7));    
    //Last 5 Average RPM Counts Eqauls....
      rpm = (rpm_array[0] + rpm_array[1] + rpm_array[2] + rpm_array[3] + rpm_array[4]) / 5;
  }
 
 }
}

void fan_interrupt()
{
   time = (micros() - time_last); 
   time_last = micros();
}
