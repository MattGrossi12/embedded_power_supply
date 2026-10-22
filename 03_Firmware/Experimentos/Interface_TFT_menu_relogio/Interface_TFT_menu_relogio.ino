#include <TFT_eSPI.h> // Biblioteca principal
#include <WiFi.h>
#include "time.h"

// Definição do display
TFT_eSPI tft = TFT_eSPI();

// Definição das cores
#define BACKGROUND_COLOR TFT_BLACK // Fundo preto
#define TEXT_COLOR TFT_WHITE       // Texto em branco
#define BUTTON_COLOR TFT_BLUE      // Cor dos botões azul
#define BUTTON_TEXT_COLOR TFT_WHITE // Cor do texto dos botões
#define BUTTON_SELECTED_TEXT_COLOR TFT_BLACK // Cor do texto do botão selecionado

// Definição dos GPIOs para os botões físicos
#define BUTTON_DOWN_PIN 39    // Descer no menu
#define BUTTON_CONFIRM_PIN 34 // Confirmar
#define BUTTON_UP_PIN 35      // Subir no menu

// Dimensões dos botões
#define BUTTON_WIDTH  200
#define BUTTON_HEIGHT 80
#define BUTTON_SELECTED_WIDTH 240  // Largura do botão selecionado
#define BUTTON_SELECTED_HEIGHT 100 // Altura do botão selecionado

// Raio para as bordas arredondadas
#define BUTTON_RADIUS 20

// Tamanho da fonte
#define TEXT_SIZE 2 // Tamanho normal do texto
#define ENLARGED_TEXT_SIZE 2.3 // Tamanho aumentado em 15%

// Variáveis para o estado do menu e da aplicação
enum MenuOption {
  INICIAR = 0,
  SAIR = 1
};

enum AppState {
  INTRO = 0,   // Tela inicial
  MENU = 1,    // Tela com os botões "Iniciar" e "Sair"
  CLOCK = 2    // Tela com o relógio
};

MenuOption currentSelection = INICIAR;
AppState currentState = INTRO;

// Variáveis de debounce
bool lastButtonDownState = false;
bool lastButtonUpState = false;
bool lastButtonConfirmState = false;
unsigned long lastDebounceTimeDown = 0;
unsigned long lastDebounceTimeUp = 0;
unsigned long lastDebounceTimeConfirm = 0;
const unsigned long debounceDelay = 200; // 200 ms

void setup() {
  // Inicializa a comunicação serial para depuração
  Serial.begin(115200);

  // Inicializa o display
  tft.init();
  tft.setRotation(1); // Ajuste a rotação conforme necessário
  tft.fillScreen(BACKGROUND_COLOR); // Fundo preto

  // Configura os pinos dos botões físicos
  pinMode(BUTTON_DOWN_PIN, INPUT);
  pinMode(BUTTON_UP_PIN, INPUT);
  pinMode(BUTTON_CONFIRM_PIN, INPUT);

  // Exibe a tela inicial
  drawIntroScreen();
}

void loop() {
  unsigned long currentTime = millis();

  if (currentState == INTRO) {
    // Verifica se o botão "Confirmar" foi pressionado
    bool readingConfirm = digitalRead(BUTTON_CONFIRM_PIN) == HIGH;
    if (readingConfirm != lastButtonConfirmState) {
      lastDebounceTimeConfirm = currentTime;
      lastButtonConfirmState = readingConfirm;
      Serial.print("Button Confirm: ");
      Serial.println(readingConfirm);
    }
    if ((currentTime - lastDebounceTimeConfirm) > debounceDelay && lastButtonConfirmState) {
      // Botão Confirmar pressionado
      Serial.println("Iniciar a partir da tela inicial");
      currentState = MENU; // Muda o estado para o menu
      tft.fillScreen(BACKGROUND_COLOR); // Limpa a tela
      drawMenu(); // Chama a função que desenha o menu
    }
  } else if (currentState == MENU) {
    // Verifica o estado dos botões
    handleMenuNavigation(currentTime);
  } else if (currentState == CLOCK) {
    // Continuar exibindo o relógio
    showClock();
  }

  // Pequena pausa para evitar consumo excessivo de CPU
  delay(50);
}

// Função para lidar com a navegação do menu
void handleMenuNavigation(unsigned long currentTime) {
  // Leitura do botão Descer (GPIO 39)
  bool readingDown = digitalRead(BUTTON_DOWN_PIN) == HIGH;
  if (readingDown != lastButtonDownState) {
    lastDebounceTimeDown = currentTime;
    lastButtonDownState = readingDown;
  }
  if ((currentTime - lastDebounceTimeDown) > debounceDelay && lastButtonDownState) {
    // Botão Descer pressionado
    if (currentSelection < SAIR) {
      currentSelection = static_cast<MenuOption>(currentSelection + 1);
      drawMenu();
      Serial.println("Menu down");
    }
  }

  // Leitura do botão Subir (GPIO 35)
  bool readingUp = digitalRead(BUTTON_UP_PIN) == HIGH;
  if (readingUp != lastButtonUpState) {
    lastDebounceTimeUp = currentTime;
    lastButtonUpState = readingUp;
  }
  if ((currentTime - lastDebounceTimeUp) > debounceDelay && lastButtonUpState) {
    // Botão Subir pressionado
    if (currentSelection > INICIAR) {
      currentSelection = static_cast<MenuOption>(currentSelection - 1);
      drawMenu();
      Serial.println("Menu up");
    }
  }

  // Leitura do botão Confirmar (GPIO 34)
  bool readingConfirm = digitalRead(BUTTON_CONFIRM_PIN) == HIGH;
  if (readingConfirm != lastButtonConfirmState) {
    lastDebounceTimeConfirm = currentTime;
    lastButtonConfirmState = readingConfirm;
  }
  if ((currentTime - lastDebounceTimeConfirm) > debounceDelay && lastButtonConfirmState) {
    // Botão Confirmar pressionado
    if (currentSelection == INICIAR) {
      Serial.println("Iniciar selecionado");
      // Adicione a função para "Iniciar" aqui
    } else if (currentSelection == SAIR) {
      Serial.println("Sair selecionado");
      currentState = CLOCK; // Alterne para o estado do relógio
      tft.fillScreen(BACKGROUND_COLOR); // Limpa a tela
      showClock(); // Chama a função que mostra o relógio
    }
  }
}

// Função para desenhar a tela inicial
void drawIntroScreen() {
  tft.fillScreen(BACKGROUND_COLOR); // Limpa a tela com fundo preto

  // Calcula a posição central da tela
  int centerX = tft.width() / 2;

  // Linhas de texto
  String linha1 = "Trabalho de conclusao de curso";
  String linha2 = "Curso: Engenharia Eletrica";
  String linha3 = "Aluno: Matheus Grossi";
  String linha4 = "RA: 3533";
  String linha5 = "Orientador: Esp. Sergio Carlos";

  // Calcula as posições Y para cada linha para centralizar
  int startY = (tft.height() - (5 * 28)) / 2; // 28px de altura para cada linha de texto aproximada (tamanho aumentado)

  // Centraliza cada linha de texto com negrito e tamanho aumentado
  tft.setTextColor(TEXT_COLOR);
  tft.setTextSize(ENLARGED_TEXT_SIZE);  // Aumenta o tamanho em 15%
  tft.setTextFont(2);  // Definindo a fonte em negrito

  tft.setCursor(centerX - (tft.textWidth(linha1) / 2), startY);
  tft.print(linha1);
  
  tft.setCursor(centerX - (tft.textWidth(linha2) / 2), startY + 35); // Aumenta espaçamento devido ao tamanho do texto
  tft.print(linha2);

  tft.setCursor(centerX - (tft.textWidth(linha3) / 2), startY + 70);
  tft.print(linha3);

  tft.setCursor(centerX - (tft.textWidth(linha4) / 2), startY + 105);
  tft.print(linha4);

  tft.setCursor(centerX - (tft.textWidth(linha5) / 2), startY + 140);
  tft.print(linha5);

  // Desenhar o botão "Iniciar" logo abaixo
  int buttonX = centerX - (BUTTON_WIDTH / 2);
  int buttonY = startY + 180; // Colocar o botão mais abaixo

  drawButton(buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, "Iniciar", true);
}

// Função para desenhar o menu completo (segunda tela)
void drawMenu() {
  // Limpa a tela
  tft.fillScreen(BACKGROUND_COLOR);
  
  // Calcula as posições dos botões
  int centerX = tft.width() / 2;
  int centerY = tft.height() / 2;
  int spacing = 20; // Espaçamento entre os botões

  // Coordenadas para o botão "Iniciar" (acima)
  int iniciarX = centerX - (BUTTON_WIDTH / 2);
  int iniciarY = centerY - BUTTON_HEIGHT - (spacing / 2);

  // Coordenadas para o botão "Sair" (abaixo)
  int sairX = centerX - (BUTTON_WIDTH / 2);
  int sairY = centerY + (spacing / 2);

  // Desenha os botões com destaque se estiverem selecionados
  drawButton(iniciarX, iniciarY, BUTTON_WIDTH, BUTTON_HEIGHT, "Iniciar", currentSelection == INICIAR);
  drawButton(sairX, sairY, BUTTON_WIDTH, BUTTON_HEIGHT, "Sair", currentSelection == SAIR);
}

// Função para desenhar um botão com ou sem destaque
void drawButton(int x, int y, int w, int h, String label, bool selected) {
  if (selected) {
    // Aumenta o tamanho do botão quando está selecionado
    w = BUTTON_SELECTED_WIDTH;
    h = BUTTON_SELECTED_HEIGHT;
    x -= (BUTTON_SELECTED_WIDTH - BUTTON_WIDTH) / 2;  // Centraliza o botão redimensionado
    y -= (BUTTON_SELECTED_HEIGHT - BUTTON_HEIGHT) / 2; // Ajusta a posição para centralizar
  }
  
  // Desenha o retângulo arredondado do botão
  tft.fillRoundRect(x, y, w, h, BUTTON_RADIUS, BUTTON_COLOR);
  tft.drawRoundRect(x, y, w, h, BUTTON_RADIUS, TFT_WHITE);
  
  // Define a cor do texto e centraliza
  tft.setTextColor(TEXT_COLOR);
  tft.setTextSize(TEXT_SIZE);
  
  // Calcula a largura do texto
  int textWidth = tft.textWidth(label);
  
  // Centraliza o texto dentro do botão
  int textX = x + (w - textWidth) / 2;
  int textY = y + (h - 16) / 2; // Altura aproximada da fonte
  
  tft.setCursor(textX, textY);
  tft.print(label);
}

// Função para mostrar a data e hora no display
void showClock() {
  // Obtém a hora atual
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Erro ao obter a hora");
    return;
  }

  // Formata a data e a hora como string
  char timeString[64];
  strftime(timeString, sizeof(timeString), "%H:%M:%S - %d/%m/%Y", &timeinfo);

  // Limpa o fundo do relógio e mostra a hora no centro da tela
  tft.fillScreen(BACKGROUND_COLOR);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);

  // Calcula a largura do texto
  int textWidth = tft.textWidth(timeString);

  // Centraliza o texto na tela
  int textX = (tft.width() - textWidth) / 2;
  int textY = (tft.height() - 16) / 2; // Altura aproximada da fonte tamanho 2

  tft.setCursor(textX, textY);
  tft.print(timeString);
}
