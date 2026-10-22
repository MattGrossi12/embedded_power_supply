// Projeto 14 -  Controlando um servo motor com Joystick

#include <Servo.h>

Servo servo1;  // Cria um objeto servo
int portCom;  // Cria uma variável para armazenar dados do Joystick
int servoPin = 11;  // Pino digital 11 conectado no servo1

void setup(){
  servo1.attach(servoPin); 
  servo1.write(90);  // Inicia o servo1 em 90 graus
}

void loop(){
  portCom = analogRead(0); // Pino X do Joystick para receber dados
  // Faz um mapeamento dos dados recebidos
  portCom = map(portCom, 0, 1023, 0, 180);  
 
  // O servo se movimenta de acordo com a posição do Joystick
  servo1.write(portCom);  
  delay(15);  // Tempo leitura do pino analogico A0
}
