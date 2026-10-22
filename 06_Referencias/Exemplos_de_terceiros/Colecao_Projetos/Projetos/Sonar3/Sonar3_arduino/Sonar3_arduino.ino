/*Projeto Régua Ultrassônica
Em desenvolvimento por Tulio Baars
<tuliobaars@gmail.com>*/

#include <LiquidCrystal.h>
#include "Ultrasonic.h"
#define echoPin 13 //Pino 13 recebe o pulso do echo
#define trigPin 7 //Pino 12 envia o pulso para gerar o echo

// inicia a biblioteca e seta os pinos do LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

Ultrasonic ultrasonic(7,13);

void setup() {
  // função de configuração - executada uma vez
     Serial.begin(9600); //inicia a porta serial a 9600 baud
   pinMode(echoPin, INPUT); // define o pino 13 como entrada (recebe)
   pinMode(trigPin, OUTPUT); // define o pino 12 como saida (envia)
  lcd.begin(16, 2); // configura a exibição do LCD - 16 colunas por 2 linhas
  // Exibe o texto entre aspas na primeira linha do LCD
  lcd.print("Distancia:");
}

void loop() {
  // função de loop - ao acabar, é executada novamente se as condições forem atendidas (se aplicável)
    //seta o pino 12 com um pulso baixo "LOW" ou desligado ou ainda 0
    digitalWrite(trigPin, LOW);
  // delay de 2 microssegundos
    delayMicroseconds(2);
  //seta o pino 12 com pulso alto "HIGH" ou ligado ou ainda 1
    digitalWrite(trigPin, HIGH);
  //delay de 10 microssegundos
    delayMicroseconds(10);
  //seta o pino 12 com pulso baixo novamente
    digitalWrite(trigPin, LOW);
  // função Ranging, faz a conversão do tempo de
  //resposta do echo em centimetros, e armazena
  //na variavel distancia
    int distancia = (ultrasonic.Ranging(CM));

Serial.print("Distancia em CM: ");
Serial.println(distancia); //vai exibir a distância no monitor serial também, além de no LCD
delay(1000); //espera 1 segundo para fazer a leitura novamente
  // joga o cursor do LCD para coluna 0, linha 1
  // (nota: a linha 1 é a segunda, lembre-se que a contagem começa do 0
  lcd.setCursor(0, 1);
  lcd.print(distancia);
  //caso seu Sonar esteja medindo um pouco acima ou abaixo, corrija aqui, adicionando ou retirando a proporção do desvio
}
