/*********************************
 ***Autor: Leonardo Dalla Porta****
 ***********24/07/2014*************
 *************Atenção**************
 ***O Codigo esta livre para uso,**
 *desde que seja mantida sua fonte*
 ********e seu autor.**************
 ********Faça um bom uso***********
 ******Att. Equipe UsinaInfo*******
 *********************************/

#include <LiquidCrystal.h>


LiquidCrystal lcd(7, 6, 5, 4, 3, 2);


int PinSensor = 8;
int VarSensor = 0;

void setup()
{

  lcd.begin(16, 2);
  lcd.setCursor( 0, 0);
  lcd.println("   Nivel Agua   ");
  lcd.setCursor( 0, 1);
  lcd.println("Em caixa D'Agua ");
  delay(3000);
  lcd.clear();
  pinMode(13, OUTPUT);
}

void loop()
{
  VarSensor = digitalRead(PinSensor);
  lcd.setCursor( 0, 0);
  lcd.println("Sensor de Nivel ");
  lcd.setCursor( 0, 1);
  lcd.println("---> Cheio! <---");

  if (VarSensor == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("  Caixa Vazia   ");
    lcd.setCursor(0, 1);
    lcd.println("--> Enchendo <--");
    digitalWrite(13, HIGH);

  }

  else
  {
    digitalWrite(13, LOW);
  }
  delay(500);
}


