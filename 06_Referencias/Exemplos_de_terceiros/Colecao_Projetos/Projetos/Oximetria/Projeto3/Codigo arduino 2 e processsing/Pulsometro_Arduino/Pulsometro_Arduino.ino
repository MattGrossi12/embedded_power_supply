
/******** Sampleador ADC para pulsometro sencillo *********
********** Alejandro Mendez, Tutoelectro.com **************
************************ 2015 *****************************/

#define FRAME_START 0x7E //Inicio de trama, corresponde al caracter ~ 

unsigned int valorADC = 0; //Variable donde se guarda la tension del ADC
int msb,lsb; //Variables para mandar la parte alta de los 10 bits y la parte baja (most significant byte y less significant byte) ya que solo podemos enviar datos de byte en byte.



void setup() {
  
  Serial.begin(115200);  //Configuramos la UART a 115200bps
  
  while (!Serial); //Esperar a que se abra el puerto serie (Solo necesario en Leonardo)
  
  pinMode(A1,OUTPUT); // Pin A1 como salida para encender el LED
  
  analogReference(INTERNAL); //Cambia referencia del ADC a una interna para poder tener mas resolucion en señales pequeñas
  
}

void loop() {
  
   digitalWrite(A1, LOW); //Encendemos el LED
    delay(1);             // Esperamos a encender el LED
    
    
    valorADC = analogRead(A0);  //Leemos la salida del sensor.
    
    msb = valorADC >> 8 & 0xFF; //Dividimos la parte alta y la parte baja de los 10 bits del ADC
    lsb = valorADC & 0xFF;      // Parte baja
   digitalWrite(A1, HIGH);      // Apagamos el LED.
   Serial.write(FRAME_START);          // Enviamos el inicio de trama 
   Serial.write(msb);           // Enviamos el byte mas significativo
   Serial.write(lsb);           // Enviamos el byte menos significativo
    
}
