/*
   Curso de Arduino WR Kits Aula 33
   
   www.wrkits.com.br
   facebook.com/wrkits
   youtube.com/user/canalwrkits
   
   Çendo de um Rotary Encoder
   
   Autor: Eng. Wagner Rambo, Data: Setembro de 2015

*/

// --- Bibliotecas Auxiliares ---
#include <LiquidCrystal.h>                              //Biblioteca para o display LCD

// --- Mapeamento de Hardware ---
#define butUp    12                                     //Botão para selecionar tela acima no digital 12
#define butDown  11                                     //Botão para selecionar tela abaixo no digital 11
#define butP     10                                     //Botão de ajuste mais no digital 10
#define butM      8                                     //Botão de ajuste menos no digital 9
#define Lamp1    A0                                     //Saída para lâmpada 1 no A0 (será usado como digital)
#define Lamp2    A1                                     //Saída para lâmpada 2 no A1 (será usado como digital)
#define motor    A2                                     //Saída para motor
#define pwmOut    9                                     //Saída para o pwm

// --- Protótipo das Funções Auxiliares ---
void changeMenu();                                      //Função para modificar o menu atual
void dispMenu();                                        //Função para mostrar o menu atual
void data_hora();                                       //Função do menu1, data e hora
void temperatura();                                     //Função do menu2, temperatura
void lights();                                          //Função do menu3, acionamento de lampadas
void menu4();                                           //Função do menu4, ajuste de PWM
void menu5();                                           //Função do menu5, controle do tempo de acionamento de um motor
void motorControl(unsigned int seconds);                //Função que controla o tempo em que o motor fica acionado
char read_encoder();                                    //Função para leitura de um rotary encoder
void displayEncoder();                                  //Função para exibir valor do encoder no display

// --- Variáveis Globais ---
char menu = 0x01;                                       //Variável para selecionar o menu
char set1 = 0x00, set2 = 0x00;                          //Controle das lâmpadas
int  pwm = 0x00;                                        //Variável para mostrar o valor do PWM
int  mTime = 0x01;                                      //Variável para o tempo de acionamento do motor
boolean t_butUp, t_butDown, t_butP, t_butM;             //Flags para armazenar o estado dos botões

// --- Hardware do LCD ---
LiquidCrystal disp(7,  //RS no digital 7
                   6,  //EN no digital 6
                   5,  //D4 no digital 5
                   4,  //D5 no digital 4
                   3,  //D6 no digital 3
                   2); //D7 no digital 2

// --- Configurações Iniciais ---
void setup()
{
  disp.begin(16,2);                                      //Inicializa LCD 16 x 2
  
  for(char i=10; i<13; i++) pinMode(i, INPUT_PULLUP);    //Entrada para os botões (digitais 10 a 12) com pull-ups internos
  pinMode(butM, INPUT_PULLUP);                           //Entrada para butM com pull-up interno
  pinMode(pwmOut, OUTPUT);                               //Saída para PWM
  pinMode(Lamp1, OUTPUT);                                //Configura saída para lâmpada 1
  pinMode(Lamp2, OUTPUT);                                //Configura saída para lâmpara 2
  pinMode(motor, OUTPUT);                                //Configura saida para o motor
  
  t_butUp   = 0x00;                                      //limpa flag do botão Up
  t_butDown = 0x00;                                      //limpa flag do botão Down
  t_butP    = 0x00;                                      //limpa flag do botão P
  t_butM    = 0x00;                                      //limpa flag do botão M
  
  digitalWrite(Lamp1, LOW);                              //Lâmpada 1 inicia apagada
  digitalWrite(Lamp2, LOW);                              //Lâmpada 2 inicia apagada


} //end setup


// --- Loop Infinito ---
void loop()
{
    
   
   changeMenu();
   dispMenu();

} //end loop

// --- Desenvolvimento das Funções Auxiliares ---
void changeMenu()                                       //Modifica o menu atual
{
   if(!digitalRead(butUp))   t_butUp   = 0x01;          //Botão Up pressionado? Seta flag
   if(!digitalRead(butDown)) t_butDown = 0x01;          //Botão Down pressionado? Seta flag
      
   if(digitalRead(butUp) && t_butUp)                    //Botão Up solto e flag setada?
   {                                                    //Sim...
      t_butUp = 0x00;                                   //Limpa flag
      
      disp.clear();                                     //Limpa display
      menu++;                                           //Incrementa menu
      
      if(menu > 0x05) menu = 0x01;                      //Se menu maior que 5, volta a ser 1
   
   } //end butUp
   
   if(digitalRead(butDown) && t_butDown)                //Botão Down solto e flag setada?
   {                                                    //Sim...
      t_butDown = 0x00;                                 //Limpa flag
      
      disp.clear();                                     //Limpa display
      menu--;                                           //Decrementa menu
      
      if(menu < 0x01) menu = 0x05;                      //Se menu menor que 1, volta a ser 5
   
   } //end butDown   

} //end changeMenu

void dispMenu()                                         //Mostra o menu atual
{
    switch(menu)                                        //Controle da variável menu
    {
       case 0x01:                                       //Caso 1
             displayEncoder();                          //Chama a função do rotary Encoder
             
             break;                                     //break
       case 0x02:                                       //Caso 2
             temperatura();                             //Chama a função do termômetro
             
             break;                                     //break
       case 0x03:                                       //Caso 3
             lights();                                  //Chama função para acionamento de lâmpadas
            
             break;                                     //break
       case 0x04:                                       //Caso 4
             menu4();                                   //Chama função para o menu4
             
             break;                                     //break
       case 0x05:                                       //Caso 5
             menu5();                                   //Chama função para o menu5
             
             break;                                     //break
    
    } //end switch menu

} //end dispMenu

void data_hora()                                        //Data e Hora (menu1)
{
    
   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print("Data e Hora");                           //Imprime mensagem
   disp.setCursor(0,1);                                 //Posiciona cursor na coluna 1, linha 2
   
   // Desenvolver uma função de data e hora...
   
   disp.print("27/08/15  15:14");                       //Mostrador (apenas ilustrativo......)

} //end data_hora

void temperatura()                                      //Temperatura (menu2)
{
   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print("Temperatura");                           //Imprime mensagem
   disp.setCursor(1,1);                                 //Posiciona cursor na coluna 2, linha 2
   
   // Desenvolver uma função de data e hora...
   
   disp.print("25 Celsius");                            //Mostrador (apenas ilustrativo......)   

} //end temperatura()
 
void lights()                                           //Acionamento de lâmpadas (menu3)
{
  
   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print("Acionar Lampadas");                      //Imprime mensagem do menu 3
  
  
   if(!digitalRead(butP))    t_butP    = 0x01;          //Botão P pressionado? Seta flag
   if(!digitalRead(butM))    t_butM    = 0x01;          //Botão M pressionado? Seta flag
   
   if(digitalRead(butP) && t_butP)                      //Botão P solto e flag setada?
   {                                                    //Sim...
      t_butP = 0x00;                                    //Limpa flag
      
      set1++;                                           //Incrementa set1
      
      if(set1 > 2) set1 = 0x01;                         //Se maior que 2, volta a ser 1
      
      switch(set1)                                      //Controle do set1
      {
          case 0x01:                                    //Caso 1
                disp.setCursor(0,1);                    //Posiciona cursor na coluna 1, linha 2
                disp.print("L1 on ");                   //Imprime mensagem
                digitalWrite(Lamp1, HIGH);              //Liga lâmpada 1
                break;                                  //Break
          case 0x02:                                    //Caso 2
                disp.setCursor(0,1);                    //Posiciona cursor na coluna 1, linha 2
                disp.print("L1 off");                   //Imprime mensagem
                digitalWrite(Lamp1, LOW);               //Desliga lâmpada 1
                break;                                  //Break
      
      } //end switch set1
   
   } //end butP
   
   if(digitalRead(butM) && t_butM)                      //Botão D solto e flag setada?
   {                                                    //Sim...
      t_butM = 0x00;                                    //Limpa flag
      
      set2++;                                           //Incrementa set2
      
      if(set2 > 2) set2 = 0x01;                         //Se maior que 2, volta a ser 1
      
      switch(set2)                                      //Controle do set2
      {
          case 0x01:                                    //Caso 1
                disp.setCursor(8,1);                    //Posiciona cursor na coluna 8, linha 2
                disp.print("L2 on ");                   //Imprime mensagem
                digitalWrite(Lamp2, HIGH);              //Liga lâmpada 2
                break;                                  //Break
          case 0x02:                                    //Caso 2
                disp.setCursor(8,1);                    //Posiciona cursor na coluna 8, linha 2
                disp.print("L2 off");                   //Imprime mensagem
                digitalWrite(Lamp2, LOW);               //Liga lâmpada 1
                break;                                  //Break
      
      } //end switch set1
   
   } //end butM


} //end lights

void menu4()                                            //Função genérica para um quarto menu...
{
   static unsigned char counter = 0; //variável para armazenar valor do encoder
   char tmpdata;
        
   tmpdata = read_encoder();
        
   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print("Set PWM");                               //Imprime mensagem
   
    if( tmpdata ) 
    {
          disp.clear();
          counter += tmpdata;
       
    } //end if
      

   pwm = counter;                                       //Atualiza PWM
   
   analogWrite(pwmOut, pwm);                            //Gera sinal pwm na saída
   
   disp.setCursor(0,1);                                 //Posiciona cursor na coluna 1, linha 2
   disp.print(pwm);                                     //Mostra PWM  


} //end menu4



void menu5()                                            //Função de controle do motor
{
  
   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print("Set Motor Time");                        //Imprime mensagem
   
   if(!digitalRead(butP))    t_butP    = 0x01;          //Botão P pressionado? Seta flag
   if(!digitalRead(butM))    t_butM    = 0x01;          //Botão M pressionado? Seta flag
   
   if(digitalRead(butP) && t_butP)                      //Botão P solto e flag setada?
   {                                                    //Sim...
      t_butP = 0x00;                                    //Limpa flag
      
      mTime++;                                          //Incrementa mTime
      
      if(mTime > 120) mTime = 0;                        //Se maior que 120, volta a ser 0
      
      disp.clear();                                     //limpa display
         
   } //end butP
   
   if(digitalRead(butM) && t_butM)                      //Botão D solto e flag setada?
   {                                                    //Sim...
      t_butM = 0x00;                                    //Limpa flag
     
      
      motorControl(mTime);                              //Chama função de controle do motor com o parâmetro mTime
      
      
       disp.clear();                                    //limpa display
          
   } //end butM

   digitalWrite(motor, LOW);
   disp.setCursor(0,1);                                 //Posiciona cursor na coluna 1, linha 2
   disp.print(mTime);                                   //Mostra valor de mTime no display  


} //end menu5


void motorControl(unsigned int seconds)
{
  char i = 0x00;                                        //Variável de iterações
  
  for(i=0;i<seconds;i++)                                //Executa enquanto i for menor que o parâmetro seconds
  {
     digitalWrite(motor, HIGH);                         //Coloca saída em high
     
     delay(1000);                                       //Tempo mínimo: 1 segundo
  } //end for
  
  
} //end motorControl


void displayEncoder()
{
        static unsigned char counter = 0; //variável para armazenar valor do encoder
        char tmpdata;
        
        tmpdata = read_encoder();
       
        disp.setCursor(0,0);
        disp.print("Encoder Test");                  //Imprime mensagem na coluna 1, linha 1
        
        disp.setCursor(6,1);                         //Posiciona cursor na coluna 1, linha 2
        disp.print(counter);                         //Mostra valor de mTime no display 
        
        if( tmpdata ) 
        {
            disp.clear();
            counter += tmpdata;
       
        } //end if

} //end disp encoder 


char read_encoder()                                    //Função para leitura de Rotary Encoder
{
  static char enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  static unsigned char old_AB = 0;
  
  old_AB <<= 2;                                        //Salva o estado anterior
  old_AB |= ( PIND & 0x03 ); // 0000 0011                      //Adiciona o estado atual
  return ( enc_states[( old_AB & 0x0f )]);
}



   //pode-se implementar quantos menus quiser
   //............


















