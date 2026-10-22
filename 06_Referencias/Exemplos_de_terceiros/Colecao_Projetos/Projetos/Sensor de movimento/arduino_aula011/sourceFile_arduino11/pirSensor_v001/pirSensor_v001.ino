#include <Servo.h>      //Inclui a biblioteca servo.h

#define flag     13     //Saída no pino digital 13
#define actuator 12     //Saída para o servo motor
#define pir       2     //Entrada para o sensor de movimento

void readSensor();      //Função auxiliar para ler o sensor

unsigned long sensorIn = 0;  //Variável para armazenar a duração do pulso
unsigned long aux;

Servo motor;                 //Configura um objeto para o servo motor

void setup()
{
   pinMode(flag, OUTPUT);     //flag será saída digital
   pinMode(actuator, OUTPUT); //actuator como saída digital
   pinMode(pir, INPUT);       //Entrada para o sensor de movimento
   
   motor.attach(12);          //Configura o pino 12 para o servo motor

   Serial.begin(9600);

}

void loop()
{
   readSensor();            //Lê o sensor
   
   if(sensorIn > 1E6 && sensorIn < 3E6) //testa se a largura do pulso está entre 1 e 3 segundos
   {
      digitalWrite(flag, HIGH); //Aciona a flag
      motor.write(180);         //Movimenta o servo em 180 graus
   
   }
   else
   {
     digitalWrite(flag, LOW); //Limpa a flag
     motor.write(90);         //Movimenta o servo em 0 graus
   
   }

}

void readSensor()
{
   
  sensorIn = pulseIn(pir, HIGH, 3E6);  //Lê a duração do pulso do sensor em nível alto e armazena em sensorIn
  
  Serial.println(sensorIn);
 // delay(1000);                    //Intervalo entre as leituras
  
}
