// Projeto 7 – Sensor de temperatura com LM35
                    
float tempC; // Cria variável para armazenar temperatura
int tempPin = 0; // Declara o pin analógico 0


void setup()               
{
  Serial.begin(9600); // Abre porta serial, inicializa dados a 9600 bps
 
}

void loop() // O código continuará a ficar em loop
{
  tempC = analogRead(tempPin); // Le o valor analógico do sensor LM35
  // Converte a entrada de temperatura em graus centigrados
  tempC = (5.0 * tempC * 100.0)/1024.0; 
  // Imprime no monitor serial - Lendo temperatura...
  Serial.println("Lendo temperatura ...  "); 
  Serial.print((byte)tempC); // Envia dados para o computador
  Serial.println(" C"); // Imprime o C de Celsius no monitor serial

  delay(1000); // Espera 1 segundos
}
