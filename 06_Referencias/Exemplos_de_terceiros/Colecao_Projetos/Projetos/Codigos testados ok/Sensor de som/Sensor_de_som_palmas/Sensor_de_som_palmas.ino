// pino 8 digital para o microfone
int microfone = A0;
//pino do relay
int relay1 = 13;

//Contador de Palmas
int contadorPalmasRelay = 0;

// Contagem para acionar o relay
int palmasRequeridaLigaRelay1 = 2;
// Contagem para desativar o relay
int  palmasRequeridaDesligaRelay1 =2;



//tempo maximo  entre o pulso seguinte
unsigned long tempoMaximoSom = 1000;
//tempo minimo entre o pulso seguinte
unsigned long tempoMinimooSom = 300;
//comprimento do som esperado
unsigned long comprimentoSonoro = 100;

unsigned long time;
unsigned long startTime = 0;

//rotina ao iniciar o programa
void setup() {                
  //inicia os pino do microfone como entrada
  pinMode(microfone, INPUT);
  //inicia os pino do relay como saida
  pinMode(relay1, OUTPUT);

  digitalWrite(relay1, HIGH); // desliga relay (o relay é invertido HIGH =desliga / LOW = liga)
}

//rotina infinita
void loop() {
  //inicia a contagem de tempo
  time = millis();
  //verifica o tempo entre o pulso inicial e o seguinte
  unsigned long tempoAposPalma = time - startTime;

  if (tempoAposPalma >= comprimentoSonoro && digitalRead(microfone) == LOW) {

    //verifica se o pulso recebido respeita o intervalo entre 1 pulso e outro
    if (tempoAposPalma < tempoMinimooSom || tempoAposPalma > tempoMaximoSom) {
      //senao for dentro do intervalo reseta a contagem e o tempo
      contadorPalmasRelay = 0;
      startTime = millis();
    }
    else {
      //se for inicia a contagem de pulso e o tempo
      contadorPalmasRelay ++;
      startTime = millis();
    }



    //verifica se a contagem de palma é igual ou superior ao numro esperado e se o rele esta desarmado
    if ((contadorPalmasRelay >= palmasRequeridaLigaRelay1 - 1) &&(digitalRead(relay1) == HIGH)) {
      //liga relay e zera contagem
      digitalWrite(relay1, LOW);
      contadorPalmasRelay = 0;
    }

    //verifica se a contagem de palma é igual ou superior ao numro esperado e se o rele esta armado
    if ((contadorPalmasRelay >= palmasRequeridaDesligaRelay1 - 1) &&(digitalRead(relay1) ==LOW)) {
      //desliga relay e zera contagem
      digitalWrite(relay1, HIGH);
      contadorPalmasRelay = 0;
    }



  }

}

