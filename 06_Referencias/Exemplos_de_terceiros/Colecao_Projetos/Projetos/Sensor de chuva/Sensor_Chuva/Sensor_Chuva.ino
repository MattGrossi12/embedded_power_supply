/*********************************
 ***Autor: Leonardo Dalla Porta****
 ***********23/07/2014*************
 *************Atenção**************
 ***O Codigo esta livre para uso,**
 *desde que seja mantida sua fonte*
 ********e seu autor.**************
 ********Faça um bom uso***********
 ******Att. Equipe UsinaInfo*******
 *********************************/
 
#include <LiquidCrystal.h>


LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int chuva; // Variavel do sensor

int PinSensor = 0;
int Porcento = 0;

void setup()
{

  lcd.begin(16, 2);
  lcd.setCursor( 0, 0);
  lcd.println(" Sensor de Chuva");
  pinMode(13, OUTPUT);

}
void loop()
{
  chuva = analogRead(PinSensor);
  Porcento = map(chuva, 1023, 0, 0, 100);


  lcd.setCursor( 1, 0);
  lcd.println("Sensor de Chuva");

  lcd.setCursor(0, 1);
  lcd.print(Porcento);
  lcd.println("%");

  delay(1000);


  if (Porcento >= 20)
  {
    lcd.setCursor(7, 1);
    lcd.println("Chovendo!");
    digitalWrite(13, HIGH);
    delay(1000);
    lcd.clear();
  }

  else
  {
    digitalWrite(13, LOW);
  }

}


