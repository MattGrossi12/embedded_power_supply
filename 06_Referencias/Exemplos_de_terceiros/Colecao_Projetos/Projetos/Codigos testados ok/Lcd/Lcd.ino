//Define a utilização da biblioteca para controle de telas LDCs.

#include <LiquidCrystal.h>
//Criando um objeto da classe LiquidCrystal e  inicializando com os pinos da interface.

LiquidCrystal lcd(2,3,4,5,6,7);
void setup() {


//Inicializando o LCD e informando o tamanho de 16 colunas e 2 linhas que é o tamanho do LCD JHD 162A usado neste projeto.

lcd.begin(16, 2);
}
void loop() {
lcd.clear(); //limpa o display do LCD.
lcd.print("Primeiro teste"); //imprime a string no display do LCD.
delay(1000);
lcd.setCursor(0,1); //posiciona cursor na coluna 0 linha 1 do LCD
lcd.print("De LCD "); //imprime a string no display do LCD.
delay(1000);
lcd.clear();
lcd.print("Teste dos caracteres de texto");
lcd.setCursor(0,1);
lcd.print("Eletrônica");
delay(1000);
lcd.clear();
lcd.print("Teste de Rolagem de texto 123456789");
delay(1000);
//Rolando o display para a esquerda 18 vezes
for (int i = 0; i <18; i++) {
lcd.scrollDisplayLeft();
delay(100);
}
delay(1000);
}
