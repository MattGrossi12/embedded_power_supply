
/******** Osciloscopio para pulsometro sencillo *********
********** Alejandro Mendez, Tutoelectro.com **************
************************ 2015 *****************************/


import processing.serial.*; //Importamos la librería para el puerto serie.

Serial serialPort;  // Creamos objeto para el puerto serie

private static final int FRAME_START = 0x7E;  //Inicio de trama.

int byteReceived, lsb, msb, value;      // Variables usadas para recibir datos.
int[] values;  //Array donde se guardan los datos a pintar en pantalla


void setup() 
{
  size(1000 ,700); // Tamaño de la ventna.
 
  serialPort = new Serial(this, Serial.list()[0] , 115200); // Abrimos el puerto serie. Podemos sustituir Serial.list()[0] por el nombre del puerto que queremos abrir , por ejemplo "COM3".
  stroke(220, 0, 0);   // Color de la linea.
  strokeWeight(1.2);  //Grosor de la línea.
  values = new int[width]; // Inicializamos el array con el ancho de la pantalla.
  smooth(); // Suaviza las lineas pintadas.
  
}

int getY(int val) {  // Sirve para calcular la altura de cada punto con respecto a la altura de la pantalla.
  return (int)(val /1024.0f * height) - 1; 
}

void draw()
{
   
  while (serialPort.available() > 3) {   //Esperamos a recibir 3 datos, Inicio de trama y dos bytes de datos.
    
      byteReceived = serialPort.read();   //Leemos el primero
      
      if (byteReceived == FRAME_START){ //Comprobamos si el primer dato es el inicio de trama.
        msb = serialPort.read();        //Leemos los datos del ADC.
        lsb = serialPort.read(); 
      }
      
      value = (msb << 8) | lsb; // Convertimos los dos bytes recibidos en un entero de 10 bits. (Variable de 16bits)
      
      System.out.println(value);  //Imprimimos en la consola de processing el valor que recibimos para asegurarnos de que va todo bien.
  }
  
  
  // Pintamos los puntos en la pantalla.
  for (int i=0; i<width-1; i++)
    values[i] = values[i+1];
  values[width-1] = value;
  background(0);
  
  for (int x=1; x<width; x++) {
    line(width-x,   height-1-getY(values[x-1]), 
         width-1-x, height-1-getY(values[x]));
  }

}

  

