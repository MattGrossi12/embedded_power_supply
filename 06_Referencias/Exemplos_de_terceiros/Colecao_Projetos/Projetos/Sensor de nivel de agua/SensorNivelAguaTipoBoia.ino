/*********************************
 ***Autor: Leonardo Dalla Porta****
 ***********08/08/2014*************
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
int PinBuzzer = 9;

void setup()
{

  lcd.begin(16, 2);
  lcd.setCursor( 0, 0);
  lcd.print("  Alarme por ");
  lcd.setCursor( 0, 1);
  lcd.print("Movimento d'agua");
  delay(3000);
  lcd.clear();
}

void loop()
{
  VarSensor = digitalRead(PinSensor);
  lcd.setCursor( 0, 0);
  lcd.print("Agua sem ");
  lcd.setCursor( 0, 1);
  lcd.print("movimento");

  if (VarSensor == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Atençao!!!");
    lcd.setCursor(0, 1);
    lcd.print("Movimento n'Agua");
    digitalWrite(PinBuzzer, HIGH);
    delay(1000);
    digitalWrite(PinBuzzer, LOW);
  }

  else
  {
    digitalWrite(PinBuzzer, LOW);
  }
  delay(500);
}


