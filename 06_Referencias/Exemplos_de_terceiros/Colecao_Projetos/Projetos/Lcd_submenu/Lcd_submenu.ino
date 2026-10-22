
// Lcd e submenu
//   NJarpa
//   Led Control using multiple screens and submenus

#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Arduino pins to lcd pins (RS,E,D4,D5,D6,D7)

//Counters to change positions of pages and sub-menus

int page_counter=1 ;       //To move beetwen pages
int subpage_counter=0;    //To move submenu 1 RGB
int subpage2_counter=0;   //To move submenu 2 Led


//-------Pins-----//
int up = 8;               //Up/Yes button 
int sel = 9;             //Select button
int down = 10;           //Down/No button
int led = 11;            //Led (pwm output)
int red = 13;            //red color
int green =12;           //green color
int blue = 19;           //blue color (A5 as output)
//---------Storage states of buttons for debounce function-----//
boolean current_up = LOW;          
boolean last_up=LOW;            
boolean current_sel = LOW;
boolean last_sel = LOW;
boolean last_down = LOW;
boolean current_down = LOW;
//--------Led states-----------//
boolean red_state = LOW;
boolean green_state = LOW;
boolean blue_state = LOW;
int led_lum =00;
        


//Custom return char
byte back[8] = {
  0b00100,
  0b01000,
  0b11111,
  0b01001,
  0b00101,
  0b00001,
  0b00001,
  0b11111
};

//Custom arrow char
byte arrow[8] = {
  0b01000,
  0b00100,
  0b00010,
  0b11111,
  0b00010,
  0b00100,
  0b01000,
  0b00000
};

void setup() {
  // Declare pin modes
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(led,OUTPUT);
  
  lcd.begin(16,2);
  lcd.createChar(1, back);
  lcd.createChar(2, arrow);
}


   //---- De-bouncing function for all buttons----//
boolean debounce(boolean last, int pin)
{
boolean current = digitalRead(pin);
if (last != current)
{
delay(5);
current = digitalRead(pin);
}
return current;
}


void loop() {

current_up = debounce(last_up, up);         //Debounce for Up button
current_sel = debounce(last_sel, sel);      //Debounce for Select  button
current_down = debounce(last_down, down);   //Debounce for Down button

//----Page counter function to move pages----//

if(subpage_counter==0 && subpage2_counter==0){                 // up/down buttons enabled if subcounters are 0,Disabled if 1,2..etc to work on submenus
  
//Page Up
    if (last_up== LOW && current_up == HIGH){ //Up button pressed
      lcd.clear();                            //Clear lcd if page is changed to print new one
      if(page_counter <3){              //Page counter never higher than 3(total of pages)
      page_counter= page_counter +1;   //Page up
      
      }
      else{
      page_counter= 3;                 //If higher than 3 (last page) stay on page 3(change to 1 if you want to rotate)
      }
  
}

 
last_up = current_up;                 //Save up button last state 

//Page Down
    if (last_down== LOW && current_down == HIGH){//Down button pressed
      lcd.clear();                       //Clear lcd if page is changed to print new one
      if(page_counter >1){              //Page counter never lower than 1 (total of pages)
      page_counter= page_counter -1;   //Page down
      
      }
      else{
      page_counter= 1;                 //If lower than 1(first page) stay on page 1(change to 3 if you want to rotate)
      }
  }
    
    last_down = current_down;         //Save down button last state
}
//------- Switch function to write and show what you want---// 
  switch (page_counter) {
   
    case 1:{     //Design of home page 1
      lcd.setCursor(5,0);
      lcd.print("Led Control");

    }//case1 end
    break;

    case 2: { //Design of page 2 RGB control

   //Static objects 
     lcd.setCursor(5,0); 
     lcd.print("RGB Led");
     lcd.setCursor(1,1);
     lcd.print("R");
     lcd.setCursor(6,1);
     lcd.print("G");
     lcd.setCursor(11,1);
     lcd.print("B");    
     lcd.setCursor(15,1);
     lcd.write(byte(1));     //Return custom char
      
     //Functions
     
     // Sub counter control
     if (last_sel== LOW && current_sel == HIGH){ //select button pressed
      if(subpage_counter <4){                    // subpage counter never higher than 4 (total of items)
     subpage_counter ++;                         //subcounter to move beetwen submenu
     }
     else{                                       //If subpage higher than 4 (total of items) return to first item
      subpage_counter=1;
     }
     }
     last_sel=current_sel;                      //Save last state of select button
     
     //First item control(subpage_counter =1) red led
     if(subpage_counter==1){
     lcd.setCursor(14,1);
     lcd.print(" ");                           //Delete last arrow position              
     lcd.setCursor(0,1);
     lcd.write(byte(2));
     if (last_up== LOW && current_up == HIGH){  //Up, red led on
     red_state=HIGH;
     }
     last_up=current_up;
     
     if(last_down== LOW && current_down == HIGH){//Down, red led off
     red_state=LOW; 
     }
     last_down=current_down;
     }

     
     if(red_state==LOW){
     lcd.setCursor(3,1);
     lcd.print("N"); 
     }
     else{                    
     lcd.setCursor(3,1);
     lcd.print("Y");
     }

     //Second item control (subpage_counter=2) green led
     if(subpage_counter==2){
     lcd.setCursor(0,1);
     lcd.print(" ");                           //Delete last arrow position                    
     lcd.setCursor(5,1);                      //Place the arrow
     lcd.write(byte(2));                      
     if (last_up== LOW && current_up == HIGH){ 
     green_state=HIGH;
     }
     last_up=current_up;
     
     if(last_down== LOW && current_down == HIGH){
     green_state=LOW; 
     }
     last_down=current_down;
     }

     
     if(green_state==LOW){
     lcd.setCursor(8,1);
     lcd.print("N"); 
     }
     else{
     lcd.setCursor(8,1);
     lcd.print("Y");
     } 

     //Thirth item control (subpage_counter=3) blue led
     if(subpage_counter==3){
     lcd.setCursor(5,1);
     lcd.print(" ");                           //Delete last arrow position                    
     lcd.setCursor(10,1);                      //Place the arrow
     lcd.write(byte(2));                      
     if (last_up== LOW && current_up == HIGH){ 
     blue_state=HIGH;
     }
     last_up=current_up;
     
     if(last_down== LOW && current_down == HIGH){
     blue_state=LOW; 
     }
     last_down=current_down;
     }

     
     if(blue_state==LOW){
     lcd.setCursor(13,1);
     lcd.print("N"); 
     }
     else{
     lcd.setCursor(13,1);
     lcd.print("Y");
     }

     //Fourth item control (subpage_counter=4) back
     if(subpage_counter==4){
     lcd.setCursor(10,1);
     lcd.print(" ");                           //Delete last arrow position                    
     lcd.setCursor(14,1);                      //Place the arrow
     lcd.write(byte(2));                      
     if (last_up== LOW && current_up == HIGH){ //Yes button pressed
     lcd.setCursor(14,1);
     lcd.print(" ");                            //Delete arrow 
     subpage_counter=0;                         //if sub page 0, exit sub menu, up/down pages enabled
     }
     last_up=current_up;
     
     if(last_down== LOW && current_down == HIGH){//No button pressed
     subpage_counter=1;                          //Stay on sub menu, return to R
     }
     last_down=current_down;
     }
    }//case2 end
    break;

    case 3: {   //Design of page 3
     //Static objects 
     lcd.setCursor(2,0);
     lcd.print("Led Brightness");
     lcd.setCursor(7,1);
     if(led_lum <10){    //To avoid "0" of number 10
     lcd.setCursor(7,1);
     lcd.print("0");
     }
     lcd.print(led_lum);
     
     lcd.setCursor(15,1);
     lcd.write(byte(1));     //Return custom char    
      
     // Sub counter 2 control
     if (last_sel== LOW && current_sel == HIGH){ //select button pressed
     if(subpage2_counter <2){                    // subpage counter never higher than 2(total of items)
     subpage2_counter ++;                         //subcounter to move beetwen submenu
     }
     else{                                       //If subpage higher than 2 (total of items) return to first item
      subpage2_counter=1;
     }
     }
     last_sel=current_sel;                      //Save last state of select button

     //First item control(subpage2_counter =1) led brightness
     if(subpage2_counter==1){
     lcd.setCursor(14,1);
     lcd.print(" ");                           //Delete last arrow position              
     lcd.setCursor(6,1);
     lcd.write(byte(2));
     
     //Control led_lum variable
     if (last_up== LOW && current_up == HIGH){  //Up bright +
     if(led_lum < 10){                         //led_lum never higher than 10 (max value)
     led_lum ++ ;
     }
     else{
     led_lum = 0;                              //If led_lum higher than 10, return to 0
     }
     }
     last_up=current_up;
     
     
     if(last_down== LOW && current_down == HIGH){  //Down bright -
     if(led_lum >0){                              //led_lum never lower than 0
     led_lum --;                                   
     }
     else{
     led_lum = 0;                                 
     }
     }
     last_down=current_down;
     
     }

     //PWM Control (pwm output 0 to 255 divided by 10 ,pin 11)
     if(led_lum==0){
     analogWrite(led,0);
     }
     if(led_lum==1){
     analogWrite(led,25);
     }
     if(led_lum==2){
     analogWrite(led,50);
     }
     if(led_lum==3){
     analogWrite(led,75);
     }
     if(led_lum==4){
     analogWrite(led,100);
     }
     if(led_lum==5){
     analogWrite(led,125);
     }
     if(led_lum==6){
     analogWrite(led,150);
     }
     if(led_lum==7){
     analogWrite(led,175);
     }
     if(led_lum==8){
     analogWrite(led,200);
     }
     if(led_lum==9){
     analogWrite(led,225);
     }
     if(led_lum==10){
     analogWrite(led,255);
     } 
        
     //Second item control (subpage2_counter==2) back
     if(subpage2_counter==2){
     lcd.setCursor(6,1);
     lcd.print(" ");                           //Delete last arrow position                    
     lcd.setCursor(14,1);                      //Place the arrow
     lcd.write(byte(2));                      
     if (last_up== LOW && current_up == HIGH){ 
     subpage2_counter=0;                       //Exit submenu, up/down pages enabled
     lcd.setCursor(14,1);
     lcd.print(" ");
     }
     last_up=current_up;
     
     if(last_down== LOW && current_down == HIGH){
     subpage2_counter=1;                        //Stay on sub menu, return to led_lum control
     }
     last_down=current_down;
     }
     
    }//case3 end
    break;
    
  }//switch end

  
//------------Change led states---------//     
if(red_state==HIGH){
  digitalWrite(red,HIGH);
}
if(red_state==LOW){
  digitalWrite(red,LOW);
}


if(green_state==HIGH){
  digitalWrite(green,HIGH);
}
if(green_state==LOW){
  digitalWrite(green,LOW);
}

if(blue_state==HIGH){
  digitalWrite(blue,HIGH);
}
if(blue_state==LOW){
  digitalWrite(blue,LOW);
}

//Show sub counters status,delete or comment if not needed
 lcd.setCursor(0,0);
 lcd.print(subpage_counter);

 lcd.setCursor(1,0);
 lcd.print(subpage2_counter);



    
    
}//loop end
