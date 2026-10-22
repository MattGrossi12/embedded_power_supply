</pre>
// Código de Exemplo para Funcionamento da Esteira Arduino MDF;
// Esteira com contagem de itens;
 
#include <LiquidCrystal.h>   // Inclui a biblioteca LiquidCrystal.h;
#define TRIGPIN 10        // Define Trig como pino Digital 10;
#define ECHOPIN 9        // Define Echo como pino Digital 9;
#define CONTAR 8         // Define variável de conta como pino Digital 8.
 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);     // Define os pinos Digitais do objeto LCD;
 
int contagem = 0, inc_dec = 1;             // Declara as variáveis iniciais de contagem;
int pin = 8;                                                // Declara pin como 8;
unsigned long dure;                               // Define as variáveis de ping;
 
void setup() {
Serial.begin (9600);                     // Define baund rate como 9600;
pinMode(TRIGPIN, OUTPUT);   // Declara TRIGPIN como um pino de saida;
pinMode(ECHOPIN, INPUT);     // Declara ECHOPIN como um pino de entrada;
pinMode(pin, INPUT);                 // Declara pin como um pino de entrada;
lcd.begin(16, 2);                           // Define as caracteres do LCD;
}
 
void loop() {
long duration, distance;               // Variável de armazenagem;
digitalWrite(TRIGPIN, LOW);       // Altera o estado de TRIGPIN;
delayMicroseconds(2);                 // Tempo de espera;
digitalWrite(TRIGPIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIGPIN, LOW);
duration = pulseIn(ECHOPIN, HIGH);   // Usa duration para ação de pulso;
distance = (duration / 2) / 29.1;
 
if (distance < 5) {                    // Se distância menor que 5:
digitalWrite(pin, HIGH);      // Quando variável estabelecida, envia sinal de contagem;
contagem += inc_dec;        // Aumenta um para cada sinal de contagem;
dure = pulseIn(pin, LOW);  // Estabelece o estado do pin;
 
if (contagem < 0) {       // Se a contagem for menor que 0;
contagem = 100;       // contagem = 100 (reinicia contagem);
}
 
if (contagem > 100)  {      // Se a contagem for mais que 100;
contagem = 0;                // Contagem = 0;
}
 
lcd.clear();                      // Limpa a tela do LCD;
lcd.setCursor(3, 0);       // Define a posição do cursor;
lcd.print("Itens:");         // Imprime "Itens:" no display;
lcd.print(contagem);    // Imprime o valor adquirido na contagem;
}
 
delay(500);   // Tempo de espera;
}
