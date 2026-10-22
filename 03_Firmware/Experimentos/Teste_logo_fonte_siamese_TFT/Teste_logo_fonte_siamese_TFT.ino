#include <TFT_eSPI.h>  // Biblioteca gráfica para ESP32 e displays TFT
#include "logofeitep.h"
#include "siamese.h"

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

  // Configura o texto para "Unifeitep"
  tft.setTextSize(6);         // Define o tamanho do texto (ajuste conforme necessário)
  tft.setTextColor(TFT_BLUE, TFT_BLACK);  // Define a cor do texto como azul e fundo preto
  
  sprite.loadFont(siamese);

  // Texto "Unifeitep"
  const char* text1 = "Unifeitep";

  // Calcula a largura e altura do texto
  uint16_t textWidth1 = tft.textWidth(text1);
  uint16_t textHeight1 = tft.fontHeight() * 6; // Multiplicado pelo tamanho do texto

  // Calcula as coordenadas para centralizar o texto
  int16_t x1 = (tft.width() - textWidth1) / 2;
  int16_t y1 = (tft.height() - textHeight1) / 2;

  // Desenha o texto "Unifeitep"
  tft.pushImage(240,160,320,80,logo_feitep_redimensionado);
  
  // Espera 2 segundos
  delay(2000);

  // Limpa a tela novamente
  tft.fillScreen(TFT_BLACK);
  Serial.println("Tela limpa para próximas informações.");

  // Configura o texto para múltiplas linhas
  tft.setTextSize(3);  // Ajusta o tamanho do texto para múltiplas linhas
  tft.setTextColor(TFT_BLUE, TFT_BLACK);  // Azul com fundo preto

  // Textos para exibir
  const char* line1 = "Trabalho de conclusao de curso";
  const char* line2 = "Fonte de bancada com controle digital";
  const char* line3 = "Matheus Grossi";
  const char* line4 = "3533";

  // Calcula as larguras dos textos
  uint16_t textWidthLine1 = tft.textWidth(line1);
  uint16_t textWidthLine2 = tft.textWidth(line2);
  uint16_t textWidthLine3 = tft.textWidth(line3);
  uint16_t textWidthLine4 = tft.textWidth(line4);

  // Define espaçamento entre as linhas
  int16_t spacing = tft.fontHeight() * 2; // Ajuste conforme necessário

  // Calcula as posições y para as linhas
  int16_t totalTextHeight = (tft.fontHeight() * 4) + (spacing * 3);
  int16_t startY = (tft.height() - totalTextHeight) / 2;

  // Linha 1
  int16_t xLine1 = (tft.width() - textWidthLine1) / 2;
  tft.setCursor(xLine1, startY);
  tft.println(line1);
  Serial.println("Exibindo linha 1.");

  // Linha 2
  int16_t xLine2 = (tft.width() - textWidthLine2) / 2;
  tft.setCursor(xLine2, startY + tft.fontHeight() + spacing);
  tft.println(line2);
  Serial.println("Exibindo linha 2.");

  // Linha 3
  int16_t xLine3 = (tft.width() - textWidthLine3) / 2;
  tft.setCursor(xLine3, startY + (tft.fontHeight() + spacing) * 2);
  tft.println(line3);
  Serial.println("Exibindo linha 3.");

  // Linha 4
  int16_t xLine4 = (tft.width() - textWidthLine4) / 2;
  tft.setCursor(xLine4, startY + (tft.fontHeight() + spacing) * 3);
  tft.println(line4);
  Serial.println("Exibindo linha 4.");

  // Desenha o botão "Iniciar" abaixo das linhas
  // Define as dimensões do botão
  int16_t buttonWidth = 200;
  int16_t buttonHeight = 60;

  // Calcula a posição do botão
  int16_t buttonX = (tft.width() - buttonWidth) / 2;
  int16_t buttonY = startY + (tft.fontHeight() + spacing) * 4 + 20; // 20 pixels abaixo da última linha

  // Desenha o retângulo do botão com cantos arredondados
  tft.fillRoundRect(buttonX, buttonY, buttonWidth, buttonHeight, 10, TFT_BLUE); // Preenche o botão com azul
  tft.drawRoundRect(buttonX, buttonY, buttonWidth, buttonHeight, 10, TFT_WHITE); // Desenha a borda branca

  // Configura o texto do botão
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE, TFT_BLUE); // Texto branco com fundo azul

  // Texto do botão
  const char* buttonText = "Iniciar";

  // Calcula a largura e altura do texto do botão
  uint16_t buttonTextWidth = tft.textWidth(buttonText);
  uint16_t buttonTextHeight = tft.fontHeight() * 3;

  // Calcula a posição para centralizar o texto no botão
  int16_t buttonTextX = buttonX + (buttonWidth - buttonTextWidth) / 2;
  int16_t buttonTextY = buttonY + (buttonHeight - buttonTextHeight) / 2;

  // Desenha o texto no botão
  tft.setCursor(buttonTextX, buttonTextY);
  tft.println(buttonText);
  Serial.println("Botão 'Iniciar' desenhado.");
}

void loop() {
  // O loop permanece vazio para que nada sobrescreva o display
}
