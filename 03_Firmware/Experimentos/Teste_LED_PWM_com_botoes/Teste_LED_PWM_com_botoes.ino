// Definindo os pinos para o LED e os botões
const int ledPin = 23;
const int buttonOnPin = 35;     // Botão para ligar o LED
const int buttonOffPin = 34;    // Botão para desligar o LED
const int buttonIncreasePin = 39; // Botão para aumentar a intensidade do LED
const int buttonDecreasePin = 36; // Botão para diminuir a intensidade do LED

int brightness = 255; // Inicia a intensidade do LED em 100% (255 para PWM)
const int step = 25; // Passo de 10% da intensidade (255 * 10% = 25)

void setup() {
  // Configura o pino do LED como saída
  pinMode(ledPin, OUTPUT);
  
  // Configura os pinos dos botões como entrada
  pinMode(buttonOnPin, INPUT);
  pinMode(buttonOffPin, INPUT);
  pinMode(buttonIncreasePin, INPUT);
  pinMode(buttonDecreasePin, INPUT);
}

void loop() {
  // Lê o estado dos botões
  int buttonOnState = digitalRead(buttonOnPin);
  int buttonOffState = digitalRead(buttonOffPin);
  int buttonIncreaseState = digitalRead(buttonIncreasePin);
  int buttonDecreaseState = digitalRead(buttonDecreasePin);

  // Se o botão do GPIO 35 for pressionado, acende o LED
  if (buttonOnState == HIGH) {
    analogWrite(ledPin, brightness);  // Ajusta o PWM de acordo com a intensidade
  }

  // Se o botão do GPIO 34 for pressionado, apaga o LED
  if (buttonOffState == HIGH) {
    analogWrite(ledPin, 0); // Coloca o PWM em 0, apagando o LED
  }

  // Se o botão de aumentar (GPIO 39) for pressionado, aumenta a intensidade em 10%
  if (buttonIncreaseState == HIGH) {
    if (brightness <= 230) { // Limita o valor máximo para 100%
      brightness += step;
    }
    analogWrite(ledPin, brightness);
    delay(200); // Pequeno atraso para evitar múltiplos incrementos rápidos
  }

  // Se o botão de diminuir (GPIO 36) for pressionado, diminui a intensidade em 10%
  if (buttonDecreaseState == HIGH) {
    if (brightness >= step) { // Evita que o valor seja menor que 0
      brightness -= step;
    }
    analogWrite(ledPin, brightness);
    delay(200); // Pequeno atraso para evitar múltiplas decrementações rápidas
  }

  // Pequeno atraso para evitar bouncing
  delay(50);
}
