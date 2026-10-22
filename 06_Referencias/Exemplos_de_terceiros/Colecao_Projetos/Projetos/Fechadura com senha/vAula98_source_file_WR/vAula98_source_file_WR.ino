/*
   Vídeo Aula de Engenharia Eletrônica
   
   Patrocínio: Toroid do Brasil (toroid.com.br)
   
   Aula 98 - Fechadura Controlada por Senha
   
    
   Autor: Eng. Wagner Rambo  Data: Dezembro de 2015
   
   www.wrkits.com.br | facebook.com/wrkits | youtube.com/user/canalwrkits
   
*/

// --- Bibliotecas Auxiliares ---
#include <LiquidCrystal.h>                              //Biblioteca para o display LCD


// --- Mapeamento de Hardware ---
#define  col_1   A1                   //coluna 1 controlada pelo pino analógico 1 (será usado como digital)
#define  col_2   2                    //coluna 2 controlada pelo pino digital 2
#define  col_3   3                    //coluna 3 controlada pelo pino digital 3
#define  row_A   4                    //linha A  controlada pelo pino digital 4
#define  row_B   5                    //linha B  controlada pelo pino digital 5
#define  row_C   6                    //linha C  controlada pelo pino digital 6
#define  row_D   7                    //linha D  controlada pelo pino digital 7

#define  buzzer  A0                   //Saída para controle do buzzer
#define  relay   A2                   //Saída para controle do relé
#define  l_Green A3                   //Saída para LED verde
#define  l_Red   A4                   //Saída para LED vermelho


// --- Protótipo das Funções Auxiliares ---
void store(char value);               //Função para armazenar o número digitado no teclado matricial
void numero();                        //Função para imprimir o número digitado na tela do LCD
void sistemaLiberado();               //Função de indicação de senha correta
void sistemaBloqueado();              //Função de indicação de senha incorreta


// --- Variáveis Globais ---
char control = 0x01;                  //variável de controle de teclado
char counter = 0x00;                  //variável auxiliar de contagem
char aux_store = 0x00;                //variável para controle de armazenamento de dígitos
char bip = 0x00;                      //variável auxiliar para limitar os bips


// -- Variáveis para armazenar a senha de 4 dígitos --
short pass1   = 0x04, pass2   = 0x03, pass3    = 0x02, pass4   = 0x01;

// -- Variáveis para armazenar a senha digitada pelo usuário --
short  number1 = 0x00, number2 = 0x00, number3 = 0x00, number4 = 0x00;

// -- Variáveis de indicação --
boolean right = 0x00, wrong = 0x00;
 


// --- Hardware do LCD ---
LiquidCrystal disp(13,  //RS no digital 13
                   12,  //EN no digital 12
                   11,  //D4 no digital 11
                   10,  //D5 no digital 10
                    9,  //D6 no digital  9
                    8); //D7 no digital  8
                    

// --- Rotina de Interrupção ---
ISR(TIMER2_OVF_vect)
{
    TCNT2=100;          // Reinicializa o registrador do Timer2
    counter++;          // incrementa counter
    
    if(counter == 0x07) // counter igual a D'8'?
    {                   // sim...
    
       counter = 0x00;  //reinicia counter
       
           
// -- Varredura do Teclado Matricial --

      if(digitalRead(col_1) && control == 0x01)        //Coluna 1 em nível high? Control igual 1?
      {                                   //Sim...
         control = 0x02;                  //control igual a 2
         digitalWrite(col_1,  LOW);       //apenas coluna 1 em nível baixo
         digitalWrite(col_2, HIGH);
         digitalWrite(col_3, HIGH);
      
      // -- Testa qual tecla foi pressionada e armazena o valor --
         if     (!digitalRead(row_A))  store(1);
         else if(!digitalRead(row_B))  store(4);
         else if(!digitalRead(row_C))  store(7);
         else if(!digitalRead(row_D))  store(11);
      
      }
      else if(digitalRead(col_2) && control == 0x02)   //Coluna 2 em nível high? Control igual 2?
      {                                   //Sim...
         control = 0x03;                  //control igual a 3
         digitalWrite(col_1, HIGH);
         digitalWrite(col_2,  LOW);       //apenas coluna 2 em nível baixo
         digitalWrite(col_3, HIGH);
         
      // -- Testa qual tecla foi pressionada e armazena o valor --
         if     (!digitalRead(row_A))  store(2);
         else if(!digitalRead(row_B))  store(5);
         else if(!digitalRead(row_C))  store(8);
         else if(!digitalRead(row_D))  store(0);

      }
      else if(digitalRead(col_3) && control == 0x03)   //Coluna 3 em nível high? Control igual 3?
      {                                   //Sim...
         control = 0x01;                  //control igual a 1     
         digitalWrite(col_1, HIGH);       //
         digitalWrite(col_2, HIGH);
         digitalWrite(col_3,  LOW);       //apenas coluna 3 em nível baixo
         
      // -- Testa qual tecla foi pressionada e armazena o valor --
         if     (!digitalRead(row_A))  store(3);
         else if(!digitalRead(row_B))  store(6);
         else if(!digitalRead(row_C))  store(9);
         else if(!digitalRead(row_D))  store(12);

      }
       
    } //end if counter
    
} //end ISR


// --- Configurações Iniciais ---
void setup()
{
     disp.begin(16,2);                                      //Inicializa LCD 16 x 2
  
     for(char i=2;i<4;i++) pinMode(i, OUTPUT);              //Saídas para varredura das colunas 2 e 3
     for(char j=4;j<8;j++) pinMode(j,  INPUT);              //Entradas para as linhas
     
     pinMode(col_1,   OUTPUT);                              //Saída para varredura da coluna 1
     pinMode(buzzer , OUTPUT);                              //Saída para o buzzer
     pinMode(relay,   OUTPUT);                              //Saída para o relé
     pinMode(l_Green, OUTPUT);                              //Saída para o LED verde de indicação
     pinMode(l_Red,   OUTPUT);                              //Saída para o LED vermelho de indicação
     
     
     digitalWrite(col_1, HIGH);                             //Inicializa coluna 1 em HIGH
     digitalWrite(col_2, HIGH);                             //Inicializa coluna 2 em HIGH
     digitalWrite(col_3, HIGH);                             //Inicializa colune 3 em HIGH
     
     digitalWrite(buzzer, LOW);                             //Buzzer desligado
     digitalWrite(relay,  LOW);                             //Relé desarmado
     digitalWrite(l_Green,LOW);                             //LED verde desligado
     digitalWrite(l_Red,  LOW);                             //LED vermelho desligado
     
     
     
     TCCR2A = 0x00;   //Timer operando em modo normal
     TCCR2B = 0x07;   //Prescaler 1:1024
     TCNT2  = 100;    //10 ms overflow again
     TIMSK2 = 0x01;   //Habilita interrupção do Timer2
 
} //end setup
 

// --- Loop Infinito ---
void loop()
{
     if(!right && !wrong) numero();     //Se right e wrong são falsos, chama função para mostrar números digitados
     
     else if(right) sistemaLiberado();  //Se right é verdadeiro, chama função que libera o sistema
    
     else if(wrong) sistemaBloqueado(); //Se wrong é verdadeiro, chama função que bloqueia o sistema
     
} //end loop



   // Estouro = Timer2_cont x prescaler x ciclo de máquina
   
   // Ciclo de máquina = 1/Fosc = 1/16E6 = 62,5ns = 62,5E-9s
   
   // Estouro = (256 - 100) x 1024 x 62,5E-9 = 9,98ms 

   // Varredura = Estouro x Counter = 9,98ms x 10 = 99,8ms
   
   
   
// --- Funções Auxiliares (Desenvolvimento) ---


void store(char value)                   //Função para armazenar o valor digitado no teclado
{
   char i;                                //variável de iterações
        
   TIMSK2 = 0x00;                         //Desabilita interrupção do Timer2
          
    
     aux_store++;                         //Incrementa aux_store
   
     digitalWrite(buzzer, HIGH);          //Bip breve no buzzer ao digitar no teclado
     delay(500);
     digitalWrite(buzzer, LOW);
     
      
         switch(aux_store)                      //controle do aux_store
         {
            case 0x01: number1 = value; break;  //caso 1, armazena valor em number1
            case 0x02: number2 = value; break;  //caso 2, armazena valor em number2
            case 0x03: number3 = value; break;  //caso 3, armazena valor em number3
            case 0x04: number4 = value; break;  //caso 4, armazena valor em number4
         
         } //end switch aux_store
         
         if(aux_store > 0x04)        //Se aux_store for maior que 4...
         {
           aux_store = 0x00;         //...volta a valer zero
           
           
           // -- Compara números, se forem todos iguais, right é verdadeiro --
           if(number1 == pass1 && number2 == pass2 && number3 == pass3 && number4 == pass4) right = 0x01;
            
           
           // -- Compara números, se um deles for diferente, wrong é verdadeiro
           if(number1 != pass1 || number2 != pass2 || number3 != pass3 || number4 != pass4) wrong = 0x01;
            
           
         } //end if aux_store
         
    
   
   TIMSK2 = 0x01;   //Habilita interrupção do Timer2
   
} //end store


void numero()                                           //Função para imprimir o número digitado na tela do LCD
{
  
  // -- String para exibir números no display --
   String password = String(number1) + String(number2) + String(number3) + String(number4);
  
   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print("Digite a Senha:");                       //Imprime mensagem
   
     
   disp.setCursor(4,1);                                 //Posiciona cursor na coluna 5, linha 2
   disp.print(password);                                //mostra senha digitada
    
    
} //end numero


void sistemaLiberado()                                  //Função de liberação do sistema
{
      
   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print(" Senha correta  ");                      //Imprime mensagem
   disp.setCursor(0,1);                                 //Posiciona cursor na coluna 1, linha 2
   disp.print("Sistema Liberado");                      //Imprime mensagem
   
   while(bip < 0x03)                                    //Enquanto bip menor que 3...
    {
         bip++;                                         //Incrementa bip
         digitalWrite(buzzer,HIGH);                     //Aciona buzzer por
         delay(500);                                    // 500 ms
         digitalWrite(buzzer, LOW);                     //Desliga buzzer por
         delay(200);                                    // 200 ms
    } //end while
    
    delay(2000);                                        //Aguarda 2 segundos
    
    digitalWrite(relay,   HIGH);                        //Aciona relé
    digitalWrite(l_Green, HIGH);                        //Liga LED verde
   

} //end sistemaLiberado


void sistemaBloqueado()                                 //Função de bloqueio do sistema
{
    
   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print(" Senha invalida ");                      //Imprime mensagem
   disp.setCursor(0,1);                                 //Posiciona cursor na coluna 1, linha 2
   disp.print("Sist. Bloqueado ");                      //Imprime mensagem
   
    while(bip < 0x04)                                   //Enquanto bip menor que 4...
    {
         bip++;                                         //Incrementa bip
         digitalWrite(buzzer,HIGH);                     //Aciona buzzer
         digitalWrite(l_Red,  LOW);                     //e desliga LED vermelho
         delay(1000);                                   //por 1 segundo
         digitalWrite(buzzer, LOW);                     //Desliga buzzer
         digitalWrite(l_Red, HIGH);                     //e liga LED vermelho
         delay(500);                                    //por meio segundo
    } //end while
    
    digitalWrite(l_Red, LOW);                           //Fica piscando LED vermelho...
    delay(400);
    digitalWrite(l_Red,HIGH);
    delay(400);
   

} //end sistemaLiberado



