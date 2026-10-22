/*
* CODIGO EXEMPLO ARDUINO SENSOR DE TENSÃO DC (SENSOR DE TENSÃO DC de 1V / 25 V) E 
SENSOR DE CORRENTE AC https://www.youtube.com/watch?v=lLV82Qvv0j4
*/


/*-IMPORTACAO DO LCD COM I2C*/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


/*DECLARACAO CONSTANTES DO LCD*/
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address



//PINO DO SENSOR ACS712
int sensorACS712 =A0;

// PINO DO SENSOR DE TENSAO ANALÓGICA(A2) UTILIZADA PELO SENSOR DE TENSÃO
int sensorTensaoDC = A1; 

// DECLARAÇÃO DE VARIÁVEL QUE RECEBE O VALOR LIDO NA PORTA ANALÓGICA A1
float valorTensaoDC; 

// DECLARAÇÃO DA VARIÁVEL DA MEDIA DE AMOSTRAGEM
int amostragem =1000;

// DECLARAÇÃO DA VARIÁVEL DA SOMA DA MEDIA DE AMOSTRAGEM
float mediaTotalTensaoDC =0;

// DECLARAÇÃO DE VARIÁVEL QUE RECEBE O VALOR FINAL CONVERTIDO EM VOLTS DC
float valorFinalTensaoDC = 0; 

//VARIAVEIS COM OS VALORES DOS RESISTORES DO SENSOR DE TENSÃO DC
float R1 = 30000.0;  
float R2 = 7500.0; 




 
// DECLARAÇÃO DA VARIÁVEL AUXILIAR
int sensorValue_aux = 0;

// DECLARAÇÃO DA VARIÁVEL QUE FARÁ LEIUTURA DIRETO NO SENSOR ACS712
float valorSensorACS712 = 0;

// DECLARAÇÃO DA VARIÁVEL QUE FARÁ LEIUTURA DE CORRENTE
float valorCorrente = 0;

// DECLARAÇÃO DA VARIÁVEL DA CONSTANTE DO ADC 5/1023
float voltsporUnidade = 0.004887586;// 5%1023

//DECLARAÇÃO DA VARIÁVEL DE SENSIBILIDADE DO ACS712
float sensibilidade = 0.100;

//DECLARAÇÃO DA VARIÁVEL DE DESVIO DO ACS712 (OPCIONAL)
float desvioACS712 = 0.045;

void setup(){
  //VALOR REFERENCIAL
  analogReference(DEFAULT);
 
 // INICIALIZA A PORTA SERIAL
  Serial.begin(9600); 
 
 // DECLARA QUE A PORTA DO SENSOR DE TENSAO DC 
  pinMode(sensorTensaoDC, INPUT);
  
  //PINO SENSOR DE CORRENTE ACS712
  pinMode(sensorACS712, INPUT);

 //INICIA O LCD DE 20 CARACTERES POR 4 LINHAS 
  lcd.begin(20,4);  
 
  //LIGA LUZ DE FUNDO
  lcd.backlight();  
  
  delay(500);
}

void loop() {
  
  
 //REINICIA O VALOR ATUAL E ATUALIZA NA PRÓXIMA LEITURA
  valorSensorACS712 =0;
  
 //REINICIA O VALOR ATUAL E ATUALIZA NA PRÓXIMA LEITURA
  valorFinalTensaoDC = 0;
 
  //REINICIA O VALOR ATUAL E ATUALIZA NA PRÓXIMA LEITURA
  mediaTotalTensaoDC = 0;
  
  //INICIA A AQUISIÇÃO DOS VALORES PARA MEDIR A CORRENTE CONSUMIDA
  //INICIA A AQUISIÇÃO DOS VALORES PARA MEDIR A TENSÃO CONSUMIDA
    for(int i=0; i < amostragem ; i++){
      // le o sensor na pino analogico A0 e ajusta o valor lido ja que a saída do sensor é 
      //(1023)vcc/2 para corrente =0
      sensorValue_aux = (analogRead(sensorACS712) -511.5); 
      
      // somam os quadrados das leituras (É valido para DC e AC).
      valorSensorACS712 += pow(sensorValue_aux,2); 
      
      //-----------------CORRENTE DC---------------------------------------
      //Somente para medir corrente DC
      ///valorSensorACS712 += sensorValue_aux ;
     
      
   //LEITURA DO VALOR NA PORTA ANALÓGICA(VARIA DE 0 ATÉ 1023)
    valorTensaoDC = analogRead(sensorTensaoDC); 
    //CALCULA O VALOR A PARTIR DA RESOLUÇÃO DO ADC
    valorTensaoDC =(valorTensaoDC*voltsporUnidade);
    //INCLUE NO CALCULO OS VALORES DO DIVISOR DE TENSÃO
    mediaTotalTensaoDC = mediaTotalTensaoDC+ (valorTensaoDC / (R2/(R1+R2)));
     
      //UM PEQUENO INTERVALO ENTRE AS LEITURAS 
      delay(1);
  }
 
 // CALCULO QUE MEDE A CORRENTE MÉDIA
 // finaliza o calculo da méida quadratica e ajusta o valor lido para volts
  valorSensorACS712 = (sqrt(valorSensorACS712/ amostragem)) * voltsporUnidade; 
  
  //-----------------CORRENTE DC---------------------------------------
  //Somente para medir corrente DC
  //valorSensorACS712 = (valorSensorACS712/ amostragem) * voltsporUnidade; 
  
  // calcula a corrente considerando a sensibilidade do sernsor (100 mV por amper)
  valorCorrente = (valorSensorACS712/sensibilidade); 
 

 
   //CASO A CORRENTE OU TENSAO SEJA NULA (ZERO) 
   //NAO IMPRIME A POTENCIA (WATTS)
   if(valorCorrente <= desvioACS712){
     valorCorrente =0;
   }

  

   //RETIRA A MEDIA DOS VALORES DE LEITURA
   valorFinalTensaoDC = mediaTotalTensaoDC / amostragem;
   




  //SAÍDA NA JANELA COM INFORMAÇÕES DA PORTA SERIAL
  Serial.print("LEITURA DE: "); 
  //SAÍDA NA JANELA COM INFORMAÇÕES DA PORTA SERIAL
  Serial.print(valorFinalTensaoDC);
  //SAÍDA NA JANELA COM INFORMAÇÕES DA PORTA SERIAL
  Serial.println(" VOLTS");
  
   //SAÍDA NA JANELA COM INFORMAÇÕES DA PORTA SERIAL
   Serial.print("Leitura: ");
   //SAÍDA NA JANELA COM INFORMAÇÕES DA PORTA SERIAL
   Serial.print(valorCorrente);
   //SAÍDA NA JANELA COM INFORMAÇÕES DA PORTA SERIAL
   Serial.println(" Amp");
  
  //LIMPA O DISPLAY DO LCD
  lcd.clear();

  //POSICIONA NA PRIMEIRA LINHA E PRIMEIRA COLUNA
  //IMPRIME A TENSAO
   lcd.setCursor(0,0); 
  //ESCREVE
   lcd.print("TENSAO :");
   lcd.print(valorFinalTensaoDC ,1);
   lcd.print(" VOLTS");
   

  //POSICIONA NA SEGUNDA LINHA E PRIMEIRA COLUNA
  //IMPRIME A CORRENTE
  
   lcd.setCursor(0,1); 
  //ESCREVE
   lcd.print("CORRENTE :");
   lcd.print(valorCorrente);
   lcd.print(" Amp");
   
  
   

   
   if((valorCorrente > 0) && (valorFinalTensaoDC > 1.15)){
       // IMPRIME POTENCIA CONSUMIDA
       lcd.setCursor(0,2); 
      //ESCREVE
       lcd.print("POTENCIA :");
       lcd.print(valorCorrente * valorFinalTensaoDC);
       
       lcd.setCursor(0,3); 
       lcd.print(" WATTS");
       
       Serial.print("POTENCIA :");
       Serial.print(valorCorrente * valorFinalTensaoDC);
       Serial.println(" WATTS");
   }
    
    
    
 
 
 delay(1000); 
}
