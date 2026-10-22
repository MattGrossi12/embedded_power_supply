#include <TFT_eSPI.h>  // Biblioteca gráfica para ESP32 e displays TFT
#include "logofeitep.h"

// Cria uma instância do display
TFT_eSPI tft = TFT_eSPI();

// Variáveis globais para o botão "Iniciar"
const int16_t buttonWidth = 200;
const int16_t buttonHeight = 60;
int16_t buttonX;
int16_t buttonY;

// Estado para verificar se a tela já foi preenchida com vermelho
bool telaVermelha = false;

// Definição dos pinos analógicos do touchscreen resistivo
const int touchPinX = 34; // GPIO 34 para X (ajuste conforme seu hardware)
const int touchPinY = 35; // GPIO 35 para Y (ajuste conforme seu hardware)

// Limite para detectar toque (ajuste conforme necessidade)
const int touchThreshold = 1000; // Valor mínimo de pressão para considerar um toque

void setup() {
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

  // Desenha o logo "Unifeitep"
  tft.pushImage((tft.width() - 320)/2, (tft.height() - 80)/2 - 100, 320, 80, logofeitep);
  
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

  // Calcula a posição do botão
  buttonX = (tft.width() - buttonWidth) / 2;
  buttonY = startY + (tft.fontHeight() + spacing) * 4 + 20; // 20 pixels abaixo da última linha

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
  // uint16_t buttonTextHeight = tft.fontHeight() * 3; // Não necessário

  // Calcula a posição para centralizar o texto no botão
  int16_t buttonTextX = buttonX + (buttonWidth - buttonTextWidth) / 2;
  int16_t buttonTextY = buttonY + (buttonHeight - tft.fontHeight()) / 2; // Centraliza verticalmente

  // Desenha o texto no botão
  tft.setCursor(buttonTextX, buttonTextY);
  tft.println(buttonText);
  Serial.println("Botão 'Iniciar' desenhado.");
}

void loop() {
  // Detecta se há um toque
  if (isTouchDetected()) {
    // Obtém as coordenadas do toque mapeadas para o display
    uint16_t x, y;
    getTouchCoordinates(x, y);

    Serial.print("Toque detectado em: ");
    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);

    // Verifica se o toque está dentro dos limites do botão "Iniciar"
    if (x >= buttonX && x <= (buttonX + buttonWidth) && y >= buttonY && y <= (buttonY + buttonHeight)) {
      Serial.println("Botão 'Iniciar' pressionado.");

      if (!telaVermelha) { // Evita reprocessar múltiplos toques
        // Preenche a tela inteira com vermelho
        tft.fillScreen(TFT_RED);
        Serial.println("Tela preenchida com vermelho.");
        telaVermelha = true;
      }
    }

    // Pequeno atraso para debounce
    delay(200);
  }

  // Pequeno atraso para evitar consumo excessivo de CPU
  delay(10);
}

// Função para detectar toque baseado em leituras analógicas
bool isTouchDetected() {
  // Leitura dos pinos de toque
  int rawX = analogRead(touchPinX);
  int rawY = analogRead(touchPinY);

  // Debug: Mostrar valores de toque
  Serial.print("Raw Touch X: ");
  Serial.print(rawX);
  Serial.print(" | Raw Touch Y: ");
  Serial.println(rawY);

  // Verifica se os valores estão abaixo do threshold (indicando toque)
  if (rawX < touchThreshold && rawY < touchThreshold) {
    return true;
  }
  return false;
}

// Função para mapear coordenadas do toque para coordenadas do display
void getTouchCoordinates(uint16_t &mappedX, uint16_t &mappedY) {
  // Leitura dos pinos de toque
  int rawX = analogRead(touchPinX);
  int rawY = analogRead(touchPinY);

  // Mapear os valores brutos para as coordenadas do display
  // **ATENÇÃO:** Estes valores de mapeamento são exemplos e devem ser ajustados conforme sua calibração
  // Você pode realizar uma calibração para obter os valores corretos
  mappedX = map(rawX, 200, 3900, 0, tft.width()); // Ajuste os valores conforme necessário
  mappedY = map(rawY, 200, 3900, 0, tft.height());

  // Limita os valores para evitar extrapolação
  if (mappedX < 0) mappedX = 0;
  if (mappedX > tft.width()) mappedX = tft.width();
  if (mappedY < 0) mappedY = 0;
  if (mappedY > tft.height()) mappedY = tft.height();

  
}
