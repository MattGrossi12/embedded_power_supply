#include <TFT_eSPI.h>  // Biblioteca gráfica para ESP32 e displays TFT
#include "logofeitep.h"

TFT_eSPI tft = TFT_eSPI();  // Cria uma instância do display
TFT_eSprite sprite = TFT_eSprite(&tft);

void setup(){
  // Inicialização da comunicação serial para depuração (opcional)
  Serial.begin(115200);
  delay(1000); // Aguarda a inicialização da comunicação serial

  // Inicializa o display
  tft.init();
  tft.setRotation(1);  // Define a rotação do display (ajuste conforme necessário)

  // Preenche a tela com a cor preta

  tft.fillScreen(TFT_BLACK);
  Serial.println("Tela preta inicializada.");

  // Espera 2 segundos
  delay(2000);

  // Desenha o texto "Unifeitep"
  tft.pushImage(240,160,600,150,logo_feitep_redimensionado);
  
  // Espera 2 segundos
  delay(2000);
  // Limpa a tela novamente
  tft.fillScreen(TFT_BLACK);
  Serial.println("Tela limpa para próximas informações."); 
}

void loop() {
  // O loop permanece vazio para que nada sobrescreva o display
}