#include <TFT_eSPI.h>
#include "siam.h"
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

void setup() {
 
  tft.init();
  tft.setRotation(1);

     sprite.createSprite(800,480); 
     sprite.loadFont(siam);
     sprite.setTextDatum(4);
}

void draw()
{
  sprite.fillSprite(TFT_BLACK);
  sprite.drawString("opa",400,240);
  sprite.pushSprite(0,0);
}

void loop() {
  draw();
}
