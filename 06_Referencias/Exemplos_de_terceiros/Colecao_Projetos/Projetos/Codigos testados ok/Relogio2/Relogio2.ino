#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3 , 4, 5, 6, 7);

//LiquidCrystal lcd(8, 9 , 4, 5, 6, 7);

//LiquidCrystal lcd(8, 9 , 10, 11, 12, 13);

int segundo;
int minuto;
int hora;

void setup() 
{
  
  lcd.begin(16, 2);
  lcd.setCursor(2, 0);
  lcd.print("Relogio Maicon");
   pinMode(10,INPUT);
    pinMode(11,INPUT);
  segundo = 0;
  minuto = 40;
  hora = 22;
} 

void loop() {
  if(digitalRead(10) == 1){
    hora++;
  }
  if(digitalRead(11) == 1){
    minuto++;
  }

  lcd.setCursor(4,1);
  if(hora < 10){
    lcd.print("0");
  }
  if(hora > 24){
    hora = 0;
  }
  lcd.print(hora);
  lcd.print(":");
   
  if(minuto < 10){
    lcd.print("0");
  }
  lcd.print(minuto);
  lcd.print(":");
  
  if(segundo < 10){
    lcd.print("0");
  }
  lcd.print(segundo);
  
  segundo++;
  if(segundo == 60){
    minuto++;
    segundo = 0;
  }
  if(minuto == 60){
    hora++;
    minuto = 0;
  }
 
  if(hora == 24){
    hora = 0;
  } 
  delay(1000);
}


