#include <TFT_eSPI.h> // Biblioteca para display TFT
#include <SPI.h>

// Definições dos pinos dos botões
#define BUTTON_UP_PIN     34
#define BUTTON_DOWN_PIN   5
#define BUTTON_SELECT_PIN 35

// Definição do pino do LED
#define LED_PIN 23

// Inicialização do display
TFT_eSPI tft = TFT_eSPI(); // Objeto TFT

// Estados do menu
enum MenuState {
  INITIAL_SCREEN,
  MAIN_MENU,
  OPTION_1_MENU,
  ADJUST_VOLTAGE,
  SHOW_X
};

// Estado atual do menu
MenuState currentState = INITIAL_SCREEN;

// Seleções dos menus
int mainMenuSelection = 0;      // 0 a 4
int option1MenuSelection = 0;   // 0 a 3
int adjustVoltageSelection = 0; // 0 a 13

// Variável para controlar o brilho do LED (0 a 255)
int ledBrightness = 128;

// Configuração PWM para LED
const int pwmChannel = 0;
const int pwmFreq = 5000;
const int pwmResolution = 8;

// Debounce
unsigned long lastDebounceTimeUp = 0;
unsigned long lastDebounceTimeDown = 0;
unsigned long lastDebounceTimeSelect = 0;
const unsigned long debounceDelay = 50;

// Para temporização do "X" na tela
unsigned long xScreenStartTime = 0;
bool isXScreenActive = false;

// Função para ler o estado dos botões com debounce
bool readButton(uint8_t pin, unsigned long &lastDebounceTimeVar) {
  bool buttonState = digitalRead(pin) == LOW; // Pressionado (pull-up)
  if (buttonState && (millis() - lastDebounceTimeVar) > debounceDelay) {
    lastDebounceTimeVar = millis();
    return true;
  }
  return false;
}

// Funções de desenho da interface
void drawInitialScreen() {
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setTextSize(3);
  tft.drawCentreString("Iniciar", tft.width() / 2, tft.height() / 2 - 20, 2);
}

void drawMainMenu() {
  tft.fillScreen(TFT_BLUE);
  tft.setTextSize(2);
  
  String menuOptions[5] = {
    "1. Fonte Regulavel 1 a 34V",
    "2. Fonte Fixa 3.3V",
    "3. Fonte Fixa 5V",
    "4. Fonte Fixa 12V",
    "5. Fonte Fixa 24V"
  };
  
  for(int i = 0; i < 5; i++) {
    if(i == mainMenuSelection){
      tft.setTextColor(TFT_YELLOW, TFT_BLUE);
    } else {
      tft.setTextColor(TFT_WHITE, TFT_BLUE);
    }
    tft.drawString(menuOptions[i], 20, 40 + i*30, LEFT);
  }
}

void drawOption1Menu() {
  tft.fillScreen(TFT_BLUE);
  tft.setTextSize(2);
  
  String option1Options[4] = {
    "1. Voltar",
    "2. Ajustar parâmetros de tensão",
    "3. Ajustar parâmetros de corrente",
    "4. Exibir gráfico da saída"
  };
  
  for(int i = 0; i < 4; i++) {
    if(i == option1MenuSelection){
      tft.setTextColor(TFT_YELLOW, TFT_BLUE);
    } else {
      tft.setTextColor(TFT_WHITE, TFT_BLUE);
    }
    tft.drawString(option1Options[i], 20, 40 + i*30, LEFT);
  }
}

void drawAdjustVoltageMenu() {
  tft.fillScreen(TFT_BLUE);
  tft.setTextSize(2);
  
  String adjustOptions[14] = {
    "1. Ligar saída",
    "2. Desligar saída",
    "3. +1%",
    "4. +5%",
    "5. +10%",
    "6. +25%",
    "7. +50%",
    "8. +100%",
    "9. -1%",
    "10. -5%",
    "11. -10%",
    "12. -25%",
    "13. -50%",
    "14. -100%"
  };
  
  for(int i = 0; i < 14; i++) {
    if(i == adjustVoltageSelection){
      tft.setTextColor(TFT_YELLOW, TFT_BLUE);
    } else {
      tft.setTextColor(TFT_WHITE, TFT_BLUE);
    }
    tft.drawString(adjustOptions[i], 20, 20 + i*25, LEFT);
  }
}

void drawXScreen() {
  tft.fillScreen(TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.setTextSize(5);
  tft.drawCentreString("X", tft.width() / 2, tft.height() / 2 - 40, 4);
}

// Funções para controle do LED com PWM
void setupPWM() {
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(LED_PIN, pwmChannel);
  ledcWrite(pwmChannel, ledBrightness); // Define brilho inicial
}

void setLEDBrightness(int brightness) {
  ledBrightness = constrain(brightness, 0, 255);
  ledcWrite(pwmChannel, ledBrightness);
}

// Funções de navegação de menu
void navigateUp() {
  switch(currentState) {
    case MAIN_MENU:
      if(mainMenuSelection > 0) mainMenuSelection--;
      break;
    case OPTION_1_MENU:
      if(option1MenuSelection > 0) option1MenuSelection--;
      break;
    case ADJUST_VOLTAGE:
      if(adjustVoltageSelection > 0) adjustVoltageSelection--;
      break;
    default:
      break;
  }
  updateScreen();
}

void navigateDown() {
  switch(currentState) {
    case MAIN_MENU:
      if(mainMenuSelection < 4) mainMenuSelection++;
      break;
    case OPTION_1_MENU:
      if(option1MenuSelection < 3) option1MenuSelection++;
      break;
    case ADJUST_VOLTAGE:
      if(adjustVoltageSelection < 13) adjustVoltageSelection++;
      break;
    default:
      break;
  }
  updateScreen();
}

void confirmSelection() {
  switch(currentState) {
    case INITIAL_SCREEN:
      currentState = MAIN_MENU;
      break;
      
    case MAIN_MENU:
      if(mainMenuSelection == 0) { // Fonte Regulavel 1 a 34V
        currentState = OPTION_1_MENU;
      }
      else { // Outras fontes
        currentState = SHOW_X;
      }
      break;
      
    case OPTION_1_MENU:
      switch(option1MenuSelection) {
        case 0: // Voltar
          currentState = MAIN_MENU;
          break;
        case 1: // Ajustar parâmetros de tensão
          currentState = ADJUST_VOLTAGE;
          break;
        case 2: // Ajustar parâmetros de corrente (Implementar se necessário)
          // Implementação futura
          break;
        case 3: // Exibir gráfico da saída (Implementar se necessário)
          // Implementação futura
          break;
      }
      break;
      
    case ADJUST_VOLTAGE:
      handleAdjustVoltageOption();
      break;
      
    default:
      break;
  }
  updateScreen();
}

void handleAdjustVoltageOption() {
  switch(adjustVoltageSelection) {
    case 0: // Ligar saída
      setLEDBrightness(255);
      break;
    case 1: // Desligar saída
      setLEDBrightness(0);
      break;
    case 2: // +1%
      setLEDBrightness(ledBrightness + (255 * 1 / 100));
      break;
    case 3: // +5%
      setLEDBrightness(ledBrightness + (255 * 5 / 100));
      break;
    case 4: // +10%
      setLEDBrightness(ledBrightness + (255 * 10 / 100));
      break;
    case 5: // +25%
      setLEDBrightness(ledBrightness + (255 * 25 / 100));
      break;
    case 6: // +50%
      setLEDBrightness(ledBrightness + (255 * 50 / 100));
      break;
    case 7: // +100%
      setLEDBrightness(255);
      break;
    case 8: // -1%
      setLEDBrightness(ledBrightness - (255 * 1 / 100));
      break;
    case 9: // -5%
      setLEDBrightness(ledBrightness - (255 * 5 / 100));
      break;
    case 10: // -10%
      setLEDBrightness(ledBrightness - (255 * 10 / 100));
      break;
    case 11: // -25%
      setLEDBrightness(ledBrightness - (255 * 25 / 100));
      break;
    case 12: // -50%
      setLEDBrightness(ledBrightness - (255 * 50 / 100));
      break;
    case 13: // -100%
      setLEDBrightness(0);
      break;
    default:
      break;
  }
}

// Função para atualizar a tela com base no estado atual
void updateScreen() {
  switch(currentState) {
    case INITIAL_SCREEN:
      drawInitialScreen();
      break;
      
    case MAIN_MENU:
      drawMainMenu();
      break;
      
    case OPTION_1_MENU:
      drawOption1Menu();
      break;
      
    case ADJUST_VOLTAGE:
      drawAdjustVoltageMenu();
      break;
      
    case SHOW_X:
      drawXScreen();
      xScreenStartTime = millis();
      isXScreenActive = true;
      break;
      
    default:
      break;
  }
}

// Função de configuração inicial
void setup() {
  // Inicialização serial para depuração
  Serial.begin(115200);
  
  // Configuração dos pinos dos botões como entrada com pull-up
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
  
  // Configuração do LED
  pinMode(LED_PIN, OUTPUT);
  
  // Configuração PWM para LED
  setupPWM();
  
  // Inicialização do display
  tft.init();
  tft.setRotation(1); // Ajuste conforme a orientação do display
  updateScreen();
}

// Função de loop principal
void loop() {
  // Leitura dos botões com debounce
  if(readButton(BUTTON_UP_PIN, lastDebounceTimeUp)) {
    navigateUp();
    delay(200); // Pequeno delay para evitar múltiplas leituras
  }
  
  if(readButton(BUTTON_DOWN_PIN, lastDebounceTimeDown)) {
    navigateDown();
    delay(200);
  }
  
  if(readButton(BUTTON_SELECT_PIN, lastDebounceTimeSelect)) {
    confirmSelection();
    delay(200);
  }
  
  // Manter a tela do "X" por 2 segundos
  if(isXScreenActive && (millis() - xScreenStartTime >= 2000)) {
    isXScreenActive = false;
    currentState = MAIN_MENU;
    updateScreen();
  }
}
