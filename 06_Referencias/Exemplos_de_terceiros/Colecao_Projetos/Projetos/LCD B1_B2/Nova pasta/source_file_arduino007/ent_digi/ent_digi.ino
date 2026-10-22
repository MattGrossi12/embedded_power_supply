#include <LiquidCrystal.h> //Incluimos a biblioteca do LCD

#define  buttom1  6 //botao 1 no pino digital 6
#define  buttom2  7 //botao 2 no pino digital 7

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //Mapeamos o hardware do lcd

int b1 = 0, b2 = 0; //Testar o nível lógico de cada entrada (botão)

void setup() 
{
  pinMode(buttom1, INPUT);
  pinMode(buttom2, INPUT);
  
  lcd.begin(16,2);
  lcd.print("Pressione");
}

void loop()
{
  b1 = digitalRead(buttom1);
  b2 = digitalRead(buttom2);
  
  if(b1 == LOW) //O botão 1 foi pressionado?
  {
    lcd.setCursor(2,1);
    lcd.print("Opcao 1");
  }
  
  if(b2 == LOW)
  {
    lcd.setCursor(2,1);
    lcd.print("Opcao 2");
  }
  
  
}
