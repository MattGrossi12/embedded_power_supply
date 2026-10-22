#include <TFT_eSPI.h> // Biblioteca principal
#include <WiFi.h>
#include "time.h"
#include <math.h> // Biblioteca para funções matemáticas
#include <SPI.h>  // Necessário para o analógico
#include "logofeitep.h"
#define TFT_GREY 0x2104 // Cor cinza (16 bits)

// Incluir alert.h do código 2
#include "alert.h"

// Definição do display
TFT_eSPI tft = TFT_eSPI();

// Definição das cores
#define BACKGROUND_COLOR TFT_BLACK     // Fundo preto
#define TEXT_COLOR TFT_CYAN            // Texto em ciano
#define BUTTON_COLOR TFT_CYAN          // Cor dos botões dos menus
#define BUTTON_TEXT_COLOR TFT_BLACK    // Cor do texto dos botões

// **Verificação e Definição de Cores Existentes**
#ifndef TFT_RED
  #define TFT_RED 0xF800
#endif

#ifndef TFT_GREEN
  #define TFT_GREEN 0x07E0
#endif

#ifndef TFT_MAGENTA
  #define TFT_MAGENTA 0xF81F
#endif

// Definição dos GPIOs para os botões físicos (pull-down)
// Ajustes solicitados: 
// Pino 18: Descer no menu
// Pino 22: Subir no menu
// Pino 19: Confirmar

#define BUTTON_UP_PIN 22        // Subir no menu
#define BUTTON_DOWN_PIN 18      // Descer no menu
#define BUTTON_CONFIRM_PIN 19   // Confirmar (para avançar)
#define PWM_PIN 5               // Pino PWM para controle de tensão regulável

// Dimensões dos botões ajustadas para largura de 800 pixels
#define BUTTON_WIDTH_NORMAL 700      // Largura dos botões normais
#define BUTTON_HEIGHT_NORMAL 60      // Altura dos botões normais
#define BUTTON_WIDTH_SELECTED 700    // Largura do botão selecionado (igual ao normal)
#define BUTTON_HEIGHT_SELECTED 90    // Altura do botão selecionado
#define BUTTON_RADIUS 15             // Raio das bordas dos botões arredondados

// Variáveis de PWM
// Agora 12 bits => 0 a 4095
int dutyCycle = 0;  
bool pwmLigado = false;

// Credenciais Wi-Fi (se necessário)
const char* ssid = "JUJUBA";
const char* password = "12345678";

// Enumerador para o estado do menu e da aplicação
enum MenuOption {
  FONTE_REGULAVEL = 0,
  FONTES_FIXAS,
  MONITORAMENTO,
  SISTEMA_EM_REDE,
  RETORNAR
};

enum FonteRegulavelSubMenuOption {
  DEFINIR_PARAMETRO = 0,
  LIGAR_SAIDA,
  DESLIGAR_SAIDA,
  VOLTAR_AO_MENU_PRINCIPAL
};

enum RedeSubMenuOption {
  CONECTAR_WIFI = 0,
  VOLTAR
};

enum AppState {
  SHOW_IMAGE = 0,
  INTRO,
  MENU,
  SUBMENU_FONTES_FIXAS,
  SUBMENU_SISTEMA_REDE,
  SUBMENU_FONTE_REGULAVEL,
  AJUSTAR_PARAMETRO,
  CONNECTING_WIFI
};

MenuOption currentSelection = FONTE_REGULAVEL;
FonteRegulavelSubMenuOption fonteRegulavelSelection = DEFINIR_PARAMETRO;
RedeSubMenuOption redeSubMenuSelection = CONECTAR_WIFI;
AppState currentState = SHOW_IMAGE; // Inicia no estado SHOW_IMAGE

// Flags de redesenho para cada estado
bool needsRedraw = true;

// Variáveis de debounce
bool lastButtonUpState = false;
bool lastButtonDownState = false;
bool lastButtonConfirmState = false;

unsigned long lastDebounceTimeUp = 0;
unsigned long lastDebounceTimeDown = 0;
unsigned long lastDebounceTimeConfirm = 0;

const unsigned long debounceDelay = 200; // 200 ms

// Variáveis para o estado SHOW_IMAGE
unsigned long imageStartTime = 0;
bool imageDisplayed = false;

// Funções existentes
void simulateWiFiConnection();
void handleShowImage(unsigned long currentTime);
void handleIntroScreen(unsigned long currentTime);
void handleMenuNavigation(unsigned long currentTime);
void handleFixedVoltageSubMenuNavigation(unsigned long currentTime);
void handleRedeSubMenuNavigation(unsigned long currentTime);
void handleFonteRegulavelSubMenuNavigation(unsigned long currentTime);
void handleAjustarParametro(unsigned long currentTime);

void drawIntroScreen();
void drawIntroButton();
void drawMenu();
void drawMenuButton(int boxY, const char* label, bool selected);
void drawSubMenuFonteRegulavel();
void drawSubMenuFontesFixas();
void drawSubMenuRede();
void drawAjustarParametro();
void updateAjustarParametro();
void ligarSaidaPWM();
void desligarSaidaPWM();

// Funções do medidor
uint32_t runTime = -99999; // time for next update
boolean range_error = 0;

int ringMeter(int value, int vmin, int vmax, int x, int y, int r, const char *units, byte scheme);
void drawAlert(int x, int y, int side, boolean draw);
unsigned int rainbow(byte value);
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
void drawIcon(const unsigned short *icon, int16_t x, int16_t y, int8_t width, int8_t height);

// Função setup
void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(BACKGROUND_COLOR);

  tft.setSwapBytes(true);

  // Frequência 5kHz, resolução 12 bits (0-4095)
  ledcSetup(0, 5000, 12); 
  ledcAttachPin(PWM_PIN, 0);

  pinMode(BUTTON_UP_PIN, INPUT_PULLDOWN);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLDOWN);
  pinMode(BUTTON_CONFIRM_PIN, INPUT_PULLDOWN);

  currentState = SHOW_IMAGE;
  needsRedraw = true; 
}

void loop() {
  unsigned long currentTime = millis();

  if (needsRedraw) {
    switch (currentState) {
      case SHOW_IMAGE:
        // Mostra logo (feito em handleShowImage)
        break;
      case INTRO:
        drawIntroScreen();
        break;
      case MENU:
        drawMenu();
        break;
      case SUBMENU_FONTES_FIXAS:
        drawSubMenuFontesFixas();
        break;
      case SUBMENU_SISTEMA_REDE:
        drawSubMenuRede();
        break;
      case SUBMENU_FONTE_REGULAVEL:
        drawSubMenuFonteRegulavel();
        break;
      case AJUSTAR_PARAMETRO:
        drawAjustarParametro();
        break;
      case CONNECTING_WIFI:
        // Tela opcional para conectar Wi-Fi
        break;
      default:
        break;
    }
    needsRedraw = false;
  }

  // Handle state actions
  switch (currentState) {
    case SHOW_IMAGE:
      handleShowImage(currentTime);
      break;
    case INTRO:
      handleIntroScreen(currentTime);
      break;
    case MENU:
      handleMenuNavigation(currentTime);
      break;
    case SUBMENU_FONTES_FIXAS:
      handleFixedVoltageSubMenuNavigation(currentTime);
      break;
    case SUBMENU_SISTEMA_REDE:
      handleRedeSubMenuNavigation(currentTime);
      break;
    case SUBMENU_FONTE_REGULAVEL:
      handleFonteRegulavelSubMenuNavigation(currentTime);
      break;
    case AJUSTAR_PARAMETRO:
      handleAjustarParametro(currentTime);
      break;
    case CONNECTING_WIFI:
      simulateWiFiConnection();
      break;
    default:
      break;
  }
}

void handleShowImage(unsigned long currentTime) {
  if (!imageDisplayed) {
    tft.fillScreen(TFT_BLACK);
    tft.pushImage((tft.width() - 620) / 2, (tft.height() - 160) / 2, 620, 160, logo_feitep_redimensionado);
    imageStartTime = currentTime;
    imageDisplayed = true;
  }

  if (currentTime - imageStartTime >= 3000) {
    tft.fillScreen(TFT_BLACK);
    currentState = INTRO; 
    needsRedraw = true; 
  }
}

void handleIntroScreen(unsigned long currentTime) {
  bool readingConfirm = digitalRead(BUTTON_CONFIRM_PIN) == HIGH;
  if (readingConfirm != lastButtonConfirmState) {
    lastDebounceTimeConfirm = currentTime;
    lastButtonConfirmState = readingConfirm;
  }

  if ((currentTime - lastDebounceTimeConfirm) > debounceDelay && lastButtonConfirmState) {
    currentState = MENU;
    needsRedraw = true;
  }
}

void handleMenuNavigation(unsigned long currentTime) {
  bool readingUp = digitalRead(BUTTON_UP_PIN) == HIGH;
  if (readingUp != lastButtonUpState) {
    lastDebounceTimeUp = currentTime;
    lastButtonUpState = readingUp;
  }
  if ((currentTime - lastDebounceTimeUp) > debounceDelay && lastButtonUpState) {
    if (currentSelection > FONTE_REGULAVEL) {
      currentSelection = static_cast<MenuOption>(currentSelection - 1);
      needsRedraw = true;
    }
  }

  bool readingDown = digitalRead(BUTTON_DOWN_PIN) == HIGH;
  if (readingDown != lastButtonDownState) {
    lastDebounceTimeDown = currentTime;
    lastButtonDownState = readingDown;
  }
  if ((currentTime - lastDebounceTimeDown) > debounceDelay && lastButtonDownState) {
    if (currentSelection < RETORNAR) {
      currentSelection = static_cast<MenuOption>(currentSelection + 1);
      needsRedraw = true;
    }
  }

  bool readingConfirm = digitalRead(BUTTON_CONFIRM_PIN) == HIGH;
  if (readingConfirm != lastButtonConfirmState) {
    lastDebounceTimeConfirm = currentTime;
    lastButtonConfirmState = readingConfirm;
  }
  if ((currentTime - lastDebounceTimeConfirm) > debounceDelay && lastButtonConfirmState) {
    switch (currentSelection) {
      case FONTE_REGULAVEL:
        currentState = SUBMENU_FONTE_REGULAVEL;
        needsRedraw = true;
        break;
      case FONTES_FIXAS:
        currentState = SUBMENU_FONTES_FIXAS;
        needsRedraw = true;
        break;
      case MONITORAMENTO:
        // Opcional
        break;
      case SISTEMA_EM_REDE:
        currentState = SUBMENU_SISTEMA_REDE;
        needsRedraw = true;
        break;
      case RETORNAR:
        currentState = SHOW_IMAGE;
        imageDisplayed = false;
        needsRedraw = true;
        break;
      default:
        break;
    }
  }
}

void handleFixedVoltageSubMenuNavigation(unsigned long currentTime) {
  bool readingUp = digitalRead(BUTTON_UP_PIN) == HIGH;
  if (readingUp != lastButtonUpState) {
    lastDebounceTimeUp = currentTime;
    lastButtonUpState = readingUp;
  }
  if ((currentTime - lastDebounceTimeUp) > debounceDelay && lastButtonUpState) {
    if (currentSelection > 0) {
      currentSelection = static_cast<MenuOption>(currentSelection - 1);
      needsRedraw = true;
    }
  }

  bool readingDown = digitalRead(BUTTON_DOWN_PIN) == HIGH;
  if (readingDown != lastButtonDownState) {
    lastDebounceTimeDown = currentTime;
    lastButtonDownState = readingDown;
  }
  if ((currentTime - lastDebounceTimeDown) > debounceDelay && lastButtonDownState) {
    if (currentSelection < 4) { // 5 opções no submenu fontes fixas
      currentSelection = static_cast<MenuOption>(currentSelection + 1);
      needsRedraw = true;
    }
  }

  bool readingConfirm = digitalRead(BUTTON_CONFIRM_PIN) == HIGH;
  if (readingConfirm != lastButtonConfirmState) {
    lastDebounceTimeConfirm = currentTime;
    lastButtonConfirmState = readingConfirm;
  }
  if ((currentTime - lastDebounceTimeConfirm) > debounceDelay && lastButtonConfirmState) {
    // Lógica para cada opção das fontes fixas, se necessário
    // Por enquanto, apenas voltar ao menu
    currentState = MENU;
    needsRedraw = true;
  }
}

void handleRedeSubMenuNavigation(unsigned long currentTime) {
  bool readingUp = digitalRead(BUTTON_UP_PIN) == HIGH;
  if (readingUp != lastButtonUpState) {
    lastDebounceTimeUp = currentTime;
    lastButtonUpState = readingUp;
  }
  if ((currentTime - lastDebounceTimeUp) > debounceDelay && lastButtonUpState) {
    if (redeSubMenuSelection > CONECTAR_WIFI) {
      redeSubMenuSelection = static_cast<RedeSubMenuOption>(redeSubMenuSelection - 1);
      needsRedraw = true;
    }
  }

  bool readingDown = digitalRead(BUTTON_DOWN_PIN) == HIGH;
  if (readingDown != lastButtonDownState) {
    lastDebounceTimeDown = currentTime;
    lastButtonDownState = readingDown;
  }
  if ((currentTime - lastDebounceTimeDown) > debounceDelay && lastButtonDownState) {
    if (redeSubMenuSelection < VOLTAR) {
      redeSubMenuSelection = static_cast<RedeSubMenuOption>(redeSubMenuSelection + 1);
      needsRedraw = true;
    }
  }

  bool readingConfirm = digitalRead(BUTTON_CONFIRM_PIN) == HIGH;
  if (readingConfirm != lastButtonConfirmState) {
    lastDebounceTimeConfirm = currentTime;
    lastButtonConfirmState = readingConfirm;
  }

  if ((currentTime - lastDebounceTimeConfirm) > debounceDelay && lastButtonConfirmState) {
    if (redeSubMenuSelection == CONECTAR_WIFI) {
      currentState = CONNECTING_WIFI;
      needsRedraw = true;
    } else if (redeSubMenuSelection == VOLTAR) {
      currentState = MENU;
      needsRedraw = true;
    }
  }
}

void handleFonteRegulavelSubMenuNavigation(unsigned long currentTime) {
  bool readingUp = digitalRead(BUTTON_UP_PIN) == HIGH;
  if (readingUp != lastButtonUpState) {
    lastDebounceTimeUp = currentTime;
    lastButtonUpState = readingUp;
  }
  if ((currentTime - lastDebounceTimeUp) > debounceDelay && lastButtonUpState) {
    if (fonteRegulavelSelection > DEFINIR_PARAMETRO) {
      fonteRegulavelSelection = static_cast<FonteRegulavelSubMenuOption>(fonteRegulavelSelection - 1);
      needsRedraw = true;
    }
  }

  bool readingDown = digitalRead(BUTTON_DOWN_PIN) == HIGH;
  if (readingDown != lastButtonDownState) {
    lastDebounceTimeDown = currentTime;
    lastButtonDownState = readingDown;
  }
  if ((currentTime - lastDebounceTimeDown) > debounceDelay && readingDown) {
    if (fonteRegulavelSelection < VOLTAR_AO_MENU_PRINCIPAL) {
      fonteRegulavelSelection = static_cast<FonteRegulavelSubMenuOption>(fonteRegulavelSelection + 1);
      needsRedraw = true;
    }
  }

  bool readingConfirm = digitalRead(BUTTON_CONFIRM_PIN) == HIGH;
  if (readingConfirm != lastButtonConfirmState) {
    lastDebounceTimeConfirm = currentTime;
    lastButtonConfirmState = readingConfirm;
  }

  if ((currentTime - lastDebounceTimeConfirm) > debounceDelay && lastButtonConfirmState) {
    switch (fonteRegulavelSelection) {
      case DEFINIR_PARAMETRO:
        currentState = AJUSTAR_PARAMETRO;
        needsRedraw = true;
        break;
      case LIGAR_SAIDA:
        ligarSaidaPWM();
        break;
      case DESLIGAR_SAIDA:
        desligarSaidaPWM();
        break;
      case VOLTAR_AO_MENU_PRINCIPAL:
        currentState = MENU;
        needsRedraw = true;
        break;
      default:
        break;
    }
  }
}

void handleAjustarParametro(unsigned long currentTime) {
  bool dutyCycleChanged = false;

  bool readingUp = digitalRead(BUTTON_UP_PIN) == HIGH;
  if (readingUp != lastButtonUpState) {
    lastDebounceTimeUp = currentTime;
    lastButtonUpState = readingUp;
  }
  if ((currentTime - lastDebounceTimeUp) > debounceDelay && lastButtonUpState) {
    // Incrementa 1 passo (até 4095)
    if (dutyCycle < 4095) {
      dutyCycle += 1;
      ligarSaidaPWM();
      dutyCycleChanged = true;
    }
  }

  bool readingDown = digitalRead(BUTTON_DOWN_PIN) == HIGH;
  if (readingDown != lastButtonDownState) {
    lastDebounceTimeDown = currentTime;
    lastButtonDownState = readingDown;
  }
  if ((currentTime - lastDebounceTimeDown) > debounceDelay && lastButtonDownState) {
    // Decrementa 1 passo (até 0)
    if (dutyCycle > 0) {
      dutyCycle -= 1;
      ligarSaidaPWM();
      dutyCycleChanged = true;
    }
  }

  bool readingConfirm = digitalRead(BUTTON_CONFIRM_PIN) == HIGH;
  if (readingConfirm != lastButtonConfirmState) {
    lastDebounceTimeConfirm = currentTime;
    lastButtonConfirmState = readingConfirm;
  }

  if ((currentTime - lastDebounceTimeConfirm) > debounceDelay && lastButtonConfirmState) {
    currentState = SUBMENU_FONTE_REGULAVEL;
    needsRedraw = true;
  }

  if (dutyCycleChanged) {
    updateAjustarParametro();
  }
}

void simulateWiFiConnection() {
  tft.fillScreen(BACKGROUND_COLOR);
  tft.setCursor(10, tft.height() / 2 - 10);
  tft.setTextColor(TEXT_COLOR);
  tft.setTextSize(3);
  tft.print("Conectando a rede padrao Wi-Fi...");
  delay(2000);

  tft.fillScreen(BACKGROUND_COLOR);
  tft.setCursor(10, tft.height() / 2 - 10);
  tft.setTextColor(TEXT_COLOR);
  tft.setTextSize(3);
  tft.print("Conexao concluida!");
  delay(2000);

  currentState = MENU;
  needsRedraw = true;
}

void drawIntroScreen() {
  tft.fillScreen(BACKGROUND_COLOR);

  String linha1 = "Trabalho de conclusao de curso";
  String linha2 = "Curso: Engenharia Eletrica";
  String linha3 = "Aluno: Matheus Grossi";
  String linha4 = "RA: 3533";
  String linha5 = "Orientador: Esp. Sergio Carlos";

  int centerXText = tft.width() / 2;

  tft.setTextColor(TEXT_COLOR);
  tft.setTextSize(2);

  int textHeight = tft.fontHeight(2);
  int numLines = 5;
  int totalTextHeight = numLines * textHeight;
  int startY = (tft.height() - totalTextHeight) / 2 - 50;

  tft.drawCentreString(linha1, centerXText, startY + textHeight * 0, 2);
  tft.drawCentreString(linha2, centerXText, startY + textHeight * 1, 2);
  tft.drawCentreString(linha3, centerXText, startY + textHeight * 2, 2);
  tft.drawCentreString(linha4, centerXText, startY + textHeight * 3, 2);
  tft.drawCentreString(linha5, centerXText, startY + textHeight * 4, 2);

  drawIntroButton();
}

void drawIntroButton() {
  int buttonWidth = BUTTON_WIDTH_NORMAL;
  int buttonHeight = BUTTON_HEIGHT_SELECTED;
  int buttonX = (tft.width() - buttonWidth) / 2;
  int buttonY = tft.height() - buttonHeight - 40;

  tft.fillRoundRect(buttonX, buttonY, buttonWidth, buttonHeight, BUTTON_RADIUS, BUTTON_COLOR);
  tft.drawRoundRect(buttonX, buttonY, buttonWidth, buttonHeight, BUTTON_RADIUS, TFT_BLACK);

  tft.setTextSize(3);
  tft.setTextColor(BUTTON_TEXT_COLOR, BUTTON_COLOR);

  const char* buttonText = "Iniciar";
  uint16_t buttonTextWidth = tft.textWidth(buttonText);
  int16_t buttonTextX = buttonX + (buttonWidth - buttonTextWidth) / 2;
  int16_t buttonTextY = buttonY + (buttonHeight - (8 * 3)) / 2;

  tft.setCursor(buttonTextX, buttonTextY);
  tft.println(buttonText);
}

void drawMenu() {
  tft.fillScreen(BACKGROUND_COLOR);

  const char* options[] = {
    "Fonte regulavel de 1 a 30V",
    "Fontes de tensao fixa",
    "Monitoramento conjunto",
    "Sistema em rede",
    "Retornar a tela inicial"
  };

  int totalHeight = 0;
  for (int i = 0; i < 5; i++) {
    if (i == currentSelection) {
      totalHeight += BUTTON_HEIGHT_SELECTED + 20; 
    } else {
      totalHeight += BUTTON_HEIGHT_NORMAL + 20;
    }
  }

  int startY = (tft.height() - totalHeight) / 2;

  for (int i = 0; i < 5; i++) {
    bool selected = (i == currentSelection);
    int boxY = startY;
    drawMenuButton(boxY, options[i], selected);
    if (selected) {
      startY += BUTTON_HEIGHT_SELECTED + 20;
    } else {
      startY += BUTTON_HEIGHT_NORMAL + 20;
    }
  }
}

void drawMenuButton(int boxY, const char* label, bool selected) {
  int buttonWidth = selected ? BUTTON_WIDTH_SELECTED : BUTTON_WIDTH_NORMAL;
  int buttonHeight = selected ? BUTTON_HEIGHT_SELECTED : BUTTON_HEIGHT_NORMAL;
  int buttonX = (tft.width() - buttonWidth) / 2;

  uint16_t buttonColor = BUTTON_COLOR;
  uint16_t textColor = BUTTON_TEXT_COLOR;

  tft.fillRoundRect(buttonX, boxY, buttonWidth, buttonHeight,
                    BUTTON_RADIUS, buttonColor);
  tft.drawRoundRect(buttonX, boxY, buttonWidth, buttonHeight,
                    BUTTON_RADIUS, TFT_BLACK);

  tft.setTextSize(3);
  tft.setTextColor(textColor);

  int16_t buttonTextX = buttonX + (buttonWidth - tft.textWidth(label)) / 2;
  int16_t buttonTextY = boxY + (buttonHeight - (8 * 3)) / 2;

  tft.setCursor(buttonTextX, buttonTextY);
  tft.print(label);
}

void drawSubMenuFonteRegulavel() {
  tft.fillScreen(BACKGROUND_COLOR);

  const char* options[] = {
    "Definir parametro da fonte",
    "Ligar saida",
    "Desligar saida",
    "Retornar ao menu anterior"
  };

  int totalHeight = 0;
  for (int i = 0; i < 4; i++) {
    if (i == fonteRegulavelSelection) {
      totalHeight += BUTTON_HEIGHT_SELECTED + 20;
    } else {
      totalHeight += BUTTON_HEIGHT_NORMAL + 20;
    }
  }

  int startY = (tft.height() - totalHeight) / 2;

  for (int i = 0; i < 4; i++) {
    bool selected = (i == fonteRegulavelSelection);
    int boxY = startY;
    drawMenuButton(boxY, options[i], selected);
    if (selected) {
      startY += BUTTON_HEIGHT_SELECTED + 20;
    } else {
      startY += BUTTON_HEIGHT_NORMAL + 20;
    }
  }
}

void drawSubMenuFontesFixas() {
  tft.fillScreen(BACKGROUND_COLOR);

  const char* options[] = {
    "Fonte fixa de 3.3V",
    "Fonte fixa de 5V",
    "Fonte fixa de 12V",
    "Fonte fixa de 24V",
    "Voltar ao menu anterior"
  };

  int totalHeight = 0;
  for (int i = 0; i < 5; i++) {
    if (i == currentSelection) {
      totalHeight += BUTTON_HEIGHT_SELECTED + 20;
    } else {
      totalHeight += BUTTON_HEIGHT_NORMAL + 20;
    }
  }

  int startY = (tft.height() - totalHeight) / 2;

  for (int i = 0; i < 5; i++) {
    bool selected = (i == currentSelection);
    int boxY = startY;
    drawMenuButton(boxY, options[i], selected);
    if (selected) {
      startY += BUTTON_HEIGHT_SELECTED + 20;
    } else {
      startY += BUTTON_HEIGHT_NORMAL + 20;
    }
  }
}

void drawSubMenuRede() {
  tft.fillScreen(BACKGROUND_COLOR);

  const char* options[] = {
    "Conectar a rede padrao Wi-Fi",
    "Voltar"
  };

  int totalHeight = 0;
  for (int i = 0; i < 2; i++) {
    if (i == redeSubMenuSelection) {
      totalHeight += BUTTON_HEIGHT_SELECTED + 20;
    } else {
      totalHeight += BUTTON_HEIGHT_NORMAL + 20;
    }
  }

  int startY = (tft.height() - totalHeight) / 2;

  for (int i = 0; i < 2; i++) {
    bool selected = (i == redeSubMenuSelection);
    int boxY = startY;
    drawMenuButton(boxY, options[i], selected);
    if (selected) {
      startY += BUTTON_HEIGHT_SELECTED + 20;
    } else {
      startY += BUTTON_HEIGHT_NORMAL + 20;
    }
  }
}

void drawAjustarParametro() {
  tft.fillScreen(BACKGROUND_COLOR);

  String title = "Ajuste da Fonte (0 a 4095 passos)";

  tft.setTextColor(TEXT_COLOR);
  tft.setTextSize(2);

  int titleWidth = tft.textWidth(title.c_str());
  int titleX = (tft.width() - titleWidth) / 2;
  int titleY = 20;

  tft.setCursor(titleX, titleY);
  tft.print(title);

  int xpos = (tft.width() / 2) - 160;
  int ypos = 100;
  int radius = 170;

  // Agora chamamos o ringMeter com range de 0 a 4095
  ringMeter(dutyCycle, 0, 4095, xpos, ypos, radius, "Passos", 4);
}

void updateAjustarParametro() {
  int xpos = (tft.width() / 2) - 160;
  int ypos = 100;
  int radius = 170;
  
  // Atualiza o medidor com valor 0 a 4095
  ringMeter(dutyCycle, 0, 4095, xpos, ypos, radius, "Passos", 4);
}

void ligarSaidaPWM() {
  ledcWrite(0, dutyCycle);
  pwmLigado = true;
}

void desligarSaidaPWM() {
  ledcWrite(0, 0);
  pwmLigado = false;
}

// Funções do medidor
int ringMeter(int value, int vmin, int vmax, int x, int y, int r, const char *units, byte scheme)
{
  x += r;
  y += r;

  int w = r / 3; 
  int angle = 150; 
  int v = map(value, vmin, vmax, -angle, angle); 
  byte seg = 3; 
  byte inc = 6; 
  int colour = TFT_BLUE;

  for (int i = -angle + inc / 2; i < angle - inc / 2; i += inc)
  {
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (r - w) + x;
    uint16_t y0 = sy * (r - w) + y;
    uint16_t x1 = sx * r + x;
    uint16_t y1 = sy * r + y;

    float sx2 = cos((i + seg - 90) * 0.0174532925);
    float sy2 = sin((i + seg - 90) * 0.0174532925);
    int x2 = sx2 * (r - w) + x;
    int y2 = sy2 * (r - w) + y;
    int x3 = sx2 * r + x;
    int y3 = sy2 * r + y;

    if (i < v)
    {
      switch (scheme)
      {
        case 0:
          colour = TFT_RED;
          break;
        case 1:
          colour = TFT_GREEN;
          break;
        case 2:
          colour = TFT_BLUE;
          break;
        case 3:
          colour = rainbow(map(i, -angle, angle, 0, 127));
          break;
        case 4:
          colour = rainbow(map(i, -angle, angle, 70, 127));
          break;
        case 5:
          colour = rainbow(map(i, -angle, angle, 127, 63));
          break;
        default:
          colour = TFT_BLUE;
          break;
      }
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
    }
    else
    {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_GREY);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_GREY);
    }
  }

  char buf[10];
  byte len = 4;
  dtostrf(value, len, 0, buf);
  buf[len] = ' ';
  buf[len + 1] = 0;

  if (value < vmin || value > vmax)
  {
    drawAlert(x, y + 90, 50, 1);
  }
  else
  {
    drawAlert(x, y + 90, 50, 0);
  }

  tft.setTextSize(1);
  tft.setTextColor(colour, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  if (r > 84)
  {
    tft.setTextPadding(55 * 3);
    tft.drawString(buf, x, y, 8);
  }
  else
  {
    tft.setTextPadding(3 * 14);
    tft.drawString(buf, x, y, 4);
  }
  tft.setTextSize(1);
  tft.setTextPadding(0);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  if (r > 84)
    tft.drawString(units, x, y + 60, 4);
  else
    tft.drawString(units, x, y + 15, 2);

  return x + r;
}

void drawAlert(int x, int y, int side, boolean draw)
{
  if (draw && !range_error)
  {
    drawIcon(alert, x - alertWidth / 2, y - alertHeight / 2, alertWidth, alertHeight);
    range_error = 1;
  }
  else if (!draw)
  {
    tft.fillRect(x - alertWidth / 2, y - alertHeight / 2, alertWidth, alertHeight, TFT_BLACK);
    range_error = 0;
  }
}

unsigned int rainbow(byte value)
{
  byte red = 0;
  byte green = 0;
  byte blue = 0;

  byte quadrant = value / 32;

  if (quadrant == 0)
  {
    blue = 31;
    green = 2 * (value % 32);
    red = 0;
  }
  if (quadrant == 1)
  {
    blue = 31 - (value % 32);
    green = 63;
    red = 0;
  }
  if (quadrant == 2)
  {
    blue = 0;
    green = 63;
    red = value % 32;
  }
  if (quadrant == 3)
  {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#define BUFF_SIZE 64
void drawIcon(const unsigned short *icon, int16_t x, int16_t y, int8_t width, int8_t height)
{
  uint16_t pix_buffer[BUFF_SIZE]; 
  tft.startWrite();
  tft.setAddrWindow(x, y, width, height);
  uint16_t nb = ((uint16_t)height * width) / BUFF_SIZE;
  for (int i = 0; i < nb; i++)
  {
    for (int j = 0; j < BUFF_SIZE; j++)
    {
      pix_buffer[j] = pgm_read_word(&icon[i * BUFF_SIZE + j]);
    }
    tft.pushColors(pix_buffer, BUFF_SIZE);
  }
  uint16_t np = ((uint16_t)height * width) % BUFF_SIZE;
  if (np)
  {
    for (int i = 0; i < np; i++)
      pix_buffer[i] = pgm_read_word(&icon[nb * BUFF_SIZE + i]);
    tft.pushColors(pix_buffer, np);
  }
  tft.endWrite();
}
