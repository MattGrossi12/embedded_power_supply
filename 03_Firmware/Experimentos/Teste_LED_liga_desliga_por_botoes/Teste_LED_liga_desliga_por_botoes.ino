// Definindo os pinos para o LED e os botões
const int ledPin = 23;
const int buttonOnPin = 36;
const int buttonOffPin = 39;

void setup() {
  // Configura o pino do LED como saída
  pinMode(ledPin, OUTPUT);
  
  // Configura os pinos dos botões como entrada
  pinMode(buttonOnPin, INPUT);
  pinMode(buttonOffPin, INPUT);
}

void loop() {
  // Lê o estado dos botões
  int buttonOnState = digitalRead(buttonOnPin);
  int buttonOffState = digitalRead(buttonOffPin);

  // Se o botão do GPIO 36 for pressionado, acende o LED
  if (buttonOnState == HIGH) {
    digitalWrite(ledPin, HIGH);
  }

  // Se o botão do GPIO 39 for pressionado, apaga o LED
  if (buttonOffState == HIGH) {
    digitalWrite(ledPin, LOW);
  }

  // Pequeno atraso para evitar bouncing
  delay(50);
}
