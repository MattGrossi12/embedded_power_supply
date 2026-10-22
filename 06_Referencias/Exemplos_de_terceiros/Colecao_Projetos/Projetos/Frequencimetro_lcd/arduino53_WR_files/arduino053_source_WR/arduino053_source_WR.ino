/*
   Curso de Arduino e AVR WR Kits Channel
   
   Aula 53 - Frequencímetro com Arduino e LCD
   
    
   Autor: Eng. Wagner Rambo  Data: Fevereiro de 2016
   
   www.wrkits.com.br | facebook.com/wrkits | youtube.com/user/canalwrkits
   
*/

// --- Bibliotecas Auxiliares ---
#include <LiquidCrystal.h> //inclui biblioteca lcd


// --- Mapeamento de Hardware ---
#define inFreq   2  //entrada para medir a frequência no pino digital 2


// --- Hardware do LCD ---
LiquidCrystal disp(13,  //RS no digital 13
                   12,  //EN no digital 12
                   11,  //D4 no digital 11
                   10,  //D5 no digital 10
                    9,  //D6 no digital  9
                    8); //D7 no digital  8


// --- Variáveis Globais ---
long freq, counter;
int pulseCount;
boolean pulse;


// --- Configurações Iniciais ---
void setup() 
{
  
  pinMode(inFreq,INPUT);    //Configura como entrada
  
  disp.begin(16, 2);        //Display lcd 16 x 2
  disp.setCursor(2,0);      //Posiciona cursor na coluna 3, linha 1
  disp.print("Frequency");  //Escreve no display
  
  pulse = 0x01;             //Seta variável de controle
  
} //end setup


// --- Loop Infinito ---
void loop() 
{
  
  counter = millis();      //counter recebe o valor do tempo em ms
  
  if(digitalRead(inFreq))  //Entrada de frequência em nível alto?
  {                        //Sim...
  
    if(pulse) pulseCount++;  //incrementa pulseCount se variável de controle for verdadeira
     
    pulse = 0x00;            //limpa variável de controle

  }
  else                     //Não...
  {
    pulse = 0x01;          //Seta variável de controle
  }


  if(counter%200 == 0x00)  //Passaram-se 200 ms?
  {                        //Sim...
    freq = pulseCount*5;   //Atualiza frequência (200 x 5 = 1000ms)
    disp.setCursor(5,1);   //Posiciona cursor na coluna 6, linha 2
    disp.print(freq);      //Imprime valor atual da frequência
    disp.print("Hz");      //Imprime "Hz"
    
    pulseCount = 0x00;     //Reinicia contador
  }
  
  
} //end loop

