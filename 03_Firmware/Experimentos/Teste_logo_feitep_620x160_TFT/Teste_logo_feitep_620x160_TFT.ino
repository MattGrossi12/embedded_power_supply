#include <TFT_eSPI.h> 
#include "logofeitep.h"

TFT_eSPI tft = TFT_eSPI(); 


void setup() {
 tft.init();
 tft.setRotation(1);
 tft.setSwapBytes(true);
 tft.fillScreen(TFT_BLACK);

  // Define a cor do texto (branco) e a cor de fundo do texto (preto)
  tft.setTextColor(TFT_RED, TFT_BLACK); 
  
  // Define o tamanho do texto (1 a 8)
  tft.setTextSize(2); 
  
  // Define a posição do cursor (x, y)
  tft.setCursor(10, 10); 
  
  delay(1000);

  tft.pushImage(100,180,620,160,logo_feitep_redimensionado);
 

}

void loop() {


}