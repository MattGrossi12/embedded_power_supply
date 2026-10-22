// Programa: Relogio com modulo DS1307 e LCD 20x4
// Autor : FILIPEFLOP
//http://www.arduinoecia.com.br/2015/04/arduino-lcd-16x2-modulo-i2c-rtc-ds1307.html

#include "Wire.h"
#include <LiquidCrystal.h>

#define DS1307_ADDRESS 0x68

// Inicializa o LCD
LiquidCrystal lcd(2,3,4,5,6,7);

byte zero = 0x00; 

void setup()
{
 // Define o LCD com 20 colunas e 4 linhas
 lcd.begin(16,2);
 Wire.begin();
 Serial.begin(9600);
 // A linha abaixo pode ser retirada apos setar a data e hora
 //SelecionaDataeHora(); 
}

void loop()
{
 Mostrarelogio();
 delay(1000);
}

void SelecionaDataeHora() // Seta a data e a hora do DS1307
{
 byte segundos = 00; // Valores de 0 a 59
 byte minutos = 04; // Valores de 0 a 59
 byte horas = 00; // Valores de 0 a 23
 byte diadasemana = 3; // Valores de 0 a 6 (0=Domingo, 1 = Segunda...)
 byte diadomes = 25; // Valores de 1 a 31
 byte mes = 10; // Valores de 1 a 12
 byte ano = 17; // Valores de 0 a 99
 Wire.beginTransmission(DS1307_ADDRESS);
 // Stop no CI para que o mesmo possa receber os dados
 Wire.write(zero); 

 // As linhas abaixo escrevem no CI os valores de 
 // data e hora que foram colocados nas variaveis acima
 Wire.write(ConverteParaBCD(segundos));
 Wire.write(ConverteParaBCD(minutos));
 Wire.write(ConverteParaBCD(horas));
 Wire.write(ConverteParaBCD(diadasemana));
 Wire.write(ConverteParaBCD(diadomes));
 Wire.write(ConverteParaBCD(mes));
 Wire.write(ConverteParaBCD(ano));
 Wire.write(zero); //Start no CI
 Wire.endTransmission(); 
}

byte ConverteParaBCD(byte val)
{ 
 // Converte o número de decimal para BCD
 return ( (val/10*16) + (val%10) );
}

byte ConverteparaDecimal(byte val) 
{ 
 // Converte de BCD para decimal
 return ( (val/16*10) + (val%16) );
}

void Mostrarelogio()
{
 Wire.beginTransmission(DS1307_ADDRESS);
 Wire.write(zero);
 Wire.endTransmission();
 Wire.requestFrom(DS1307_ADDRESS, 7);
 int segundos = ConverteparaDecimal(Wire.read());
 int minutos = ConverteparaDecimal(Wire.read());
 int horas = ConverteparaDecimal(Wire.read() & 0b111111); 
 int diadasemana = ConverteparaDecimal(Wire.read()); 
 int diadomes = ConverteparaDecimal(Wire.read());
 int mes = ConverteparaDecimal(Wire.read());
 int ano = ConverteparaDecimal(Wire.read());
 // Imprime mensagem na primeira linha do display
 lcd.setCursor(0,0);
 lcd.print("Arduino");
 
 // Mostra a hora atual no display
 lcd.setCursor(8, 0);
 if (horas < 10)
 {lcd.print("0");}
 lcd.print(horas);
 lcd.print(":");
 if (minutos < 10)
 {lcd.print("0");}
 lcd.print(minutos);
 lcd.print(":");
 if (segundos < 10)
 {lcd.print("0");}
 lcd.print(segundos);
 
 // Mostra a data atual no display
 lcd.setCursor(0, 1);
 lcd.print(" : ");
 lcd.setCursor(0,1);
 if (diadomes < 10)
 {lcd.print("0");}
 lcd.print(diadomes);
 lcd.print(".");
 if (mes < 10)
 {lcd.print("0");}
 lcd.print(mes);
 lcd.print(".");
 lcd.print(ano);
 
 //Mostra o dia da semana no display
 lcd.setCursor(10, 1);
 switch(diadasemana)
 {
 case 0:lcd.print("Doming");
 break;
 case 1:lcd.print("Segunda");
 break;
 case 2:lcd.print("Terca");
 break;
 case 3:lcd.print("Quarta");
 break;
 case 4:lcd.print("Quinta");
 break;
 case 5:lcd.print("Sexta");
 break;
 case 6:lcd.print("Sabado");
 }
}
