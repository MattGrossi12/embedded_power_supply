/* ===============================================================================

     Termômetro Animado com Display Nextion 7" e Arduino

     Exibe temperatura e umidade relativa do ar


     autor: Eng. Wagner Rambo

     data:  Julho de 2017

  
=============================================================================== */


// ===============================================================================
// --- Biblioteca Auxiliar ---
#include "Nextion.h"      //biblioteca Nextion
#include <dht.h>          //biblioteca sensor DHT


// ===============================================================================
// --- Mapeamento de Hardware ---
#define    dht_pin    5   //pino de sinal do DHT11
 
 
// ===============================================================================
// --- Declaração de Objetos ---
dht   my_dht;        //objeto para o sensor

// -- Objetos do Nextion --
                     //page id:0, id componente:1, nome do componente: "bt0"
NexProgressBar j0 = NexProgressBar(0, 6, "j0");
NexText txt_temp  = NexText(0, 2, "t0");
NexText txt_umid  = NexText(0, 4, "t2");

NexProgressBar j1 = NexProgressBar(0, 6, "j1");
NexText txt_temp1  = NexText(0, 2, "t0");
NexText txt_umid1  = NexText(0, 4, "t2");

NexPage page0 = NexPage(0, 0, "page0");
NexPage page1 = NexPage(1, 0, "page1");


// ===============================================================================
// --- Protótipo das Funções ---
 void readTempHumi();           //Função para leitura de temperatura e umidade; e conversão

 
// ===============================================================================
// --- Variáveis Globais ---
int        temperatura = 0x00,   //armazena a temperatura em inteiro
           temp_bar    = 0x00,   //armazena a temperatura para a barra de progressão
           umidade     = 0x00;   //armazena a umidade em inteiro
  
char       txt1[10],
           txt2[10],
           txt3[10],
           txt4[10];

boolean    flag1 = 0x00, 
           flag2 = 0x00;


// ===============================================================================
// --- Configurações Iniciais ---
void setup()
{
    nexInit();                           //inicializa Nextion lib
 
    readTempHumi();                      //chama função para leitura de temperatura e umidade

    if(temperatura < 20) page0.show();   //se temperatura menor que 20 graus Celsius, inicia na página 0
    else page1.show();                   //senão, inicia na página 1

    delay(300);                          //aguarda...


} //end setup
 

// ===============================================================================
// --- Loop Infinito ---
void loop()
{
  
     readTempHumi(); //lê temperatura e umidade. Converte valores para exibir no Nextion

     //Troca para página 0 se detectar transição de 20 para 19 graus Celsius...
     if(temperatura == 20) flag1 = 0x01; 

     if(flag1 && temperatura == 19) 
     {
         page0.show();
         flag1 = 0x00;
     }


     //Troca para página 1 se detectar transição de 21 para 22 graus Celsius...
     if(temperatura == 21) flag2 = 0x01; 

     if(flag2 && temperatura == 22) 
     {
         page1.show();
         flag2 = 0x00;
     }

} //end loop


void readTempHumi()
{
  
    //Atualização dos valores do Nextion
    j0.setValue(temp_bar);
    txt_temp.setText(txt1);
    txt_umid.setText(txt2);
    
    j1.setValue(temp_bar);
    txt_temp1.setText(txt3);
    txt_umid1.setText(txt4);
 
    //Lê parâmetros de temperatura e umidade relativa do ar no DHT11
    my_dht.read11(dht_pin);
    temperatura = my_dht.temperature;
    umidade     = my_dht.humidity;
 
    //Converte inteiros para string
    memset(txt1, 0, sizeof(txt1));
    itoa(temperatura, txt1, 10);
    memset(txt2, 0, sizeof(txt2));
    itoa(umidade, txt2, 10);

    memset(txt3, 0, sizeof(txt3));
    itoa(temperatura, txt3, 10);
    memset(txt4, 0, sizeof(txt4));
    itoa(umidade, txt4, 10);
 
    //Normaliza a barra animada de acordo com a temperatura
    temp_bar = map(temperatura, -30, 45, 0, 100);
  
  
} //end readTempHumi




