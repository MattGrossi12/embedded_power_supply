#define M_SIZE 1.3333 // Define o tamanho do medidor para ajustar a rotação do display

#include <TFT_eSPI.h> // Inclui a biblioteca específica de hardware para TFT
#include <SPI.h>       // Inclui a biblioteca SPI para comunicação serial

TFT_eSPI tft = TFT_eSPI();       // Cria uma instância da biblioteca TFT_eSPI

#define TFT_GREY 0x5AEB // Define a cor cinza usando código de 16 bits

float ltx = 0;    // Coordenada x salva da base da agulha
uint16_t osx = M_SIZE*120, osy = M_SIZE*120; // Coordenadas x & y salvas para a agulha
uint32_t updateTime = 0;       // Tempo para a próxima atualização

int old_analog =  -999; // Valor anterior exibido no medidor analógico
int value[6] = {0, 0, 0, 0, 0, 0}; // Valores atuais dos 6 medidores lineares
int old_value[6] = { -1, -1, -1, -1, -1, -1}; // Valores anteriores dos 6 medidores lineares
int d = 0; // Variável para controle de ângulo no loop

void setup(void) {
  tft.init(); // Inicializa o display TFT
  tft.setRotation(1); // Define a rotação do display (1: paisagem)
  Serial.begin(57600); // Inicializa a comunicação serial para depuração
  tft.fillScreen(TFT_BLACK); // Preenche a tela com a cor preta
  analogMeter(); // Chama a função para desenhar o medidor analógico
  updateTime = millis(); // Define o tempo para a próxima atualização
}

void loop() {
  if (updateTime <= millis()) { // Verifica se é hora de atualizar os medidores
    updateTime = millis() + 35; // Define um atraso de 35ms para limitar a velocidade de atualização
 
    d += 4; if (d >= 360) d = 0; // Reinicia 'd' após completar um ciclo de 360 graus
    value[0] = 50 + 50 * sin((d + 0) * 0.0174532925); // Calcula valor para o medidor analógico usando seno
    
    plotNeedle(value[0], 0); // Atualiza o ponteiro do medidor analógico com o valor calculado
  }
}

void analogMeter() {
  // Contorno do medidor
  tft.setTextColor(TFT_CYAN);  // Define a cor do texto como ciano

  // Desenha marcas a cada 5 graus de -50 a +50 graus (amplitude total de 100 graus)
  for (int i = -50; i < 51; i += 5) {
    int tl = 15; // Comprimento da marca de escala longa

    // Coordenadas da marca a ser desenhada
    float sx = cos((i - 90) * 0.0174532925); // Calcula o cosseno do ângulo ajustado
    float sy = sin((i - 90) * 0.0174532925); // Calcula o seno do ângulo ajustado
    uint16_t x0 = sx * (M_SIZE*100 + tl) + M_SIZE*120; // Calcula a posição x do início da marca
    uint16_t y0 = sy * (M_SIZE*100 + tl) + M_SIZE*140; // Calcula a posição y do início da marca
    uint16_t x1 = sx * M_SIZE*100 + M_SIZE*120; // Calcula a posição x do fim da marca
    uint16_t y1 = sy * M_SIZE*100 + M_SIZE*140; // Calcula a posição y do fim da marca

    // Coordenadas da próxima marca para preenchimento de zonas
    float sx2 = cos((i + 5 - 90) * 0.0174532925); // Calcula o cosseno para a próxima marca
    float sy2 = sin((i + 5 - 90) * 0.0174532925); // Calcula o seno para a próxima marca
    int x2 = sx2 * (M_SIZE*100 + tl) + M_SIZE*120; // Posição x da próxima marca
    int y2 = sy2 * (M_SIZE*100 + tl) + M_SIZE*140; // Posição y da próxima marca
    int x3 = sx2 * M_SIZE*100 + M_SIZE*120; // Posição x do fim da próxima marca
    int y3 = sy2 * M_SIZE*100 + M_SIZE*140; // Posição y do fim da próxima marca

    // Limites da zona amarela (comentado no código original)
    if (i >= -50 && i < 0) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_YELLOW);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_YELLOW);
    }

    // Limites da zona verde
    if (i >= 0 && i < 25) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_GREEN); // Preenche triângulos verdes
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_GREEN); // Continua o preenchimento verde
    }

    // Limites da zona vermelha
    if (i >= 25 && i < 50) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_RED); // Preenche triângulos vermelhos
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_RED); // Continua o preenchimento vermelho
    }

    // Reduz o comprimento da marca de escala curta
    if (i % 25 != 0) tl = 8;

    // Recalcula as coordenadas caso o comprimento da marca tenha mudado
    x0 = sx * (M_SIZE*100 + tl) + M_SIZE*120; // Recalcula x0 com novo comprimento
    y0 = sy * (M_SIZE*100 + tl) + M_SIZE*140; // Recalcula y0 com novo comprimento
    x1 = sx * M_SIZE*100 + M_SIZE*120; // Recalcula x1
    y1 = sy * M_SIZE*100 + M_SIZE*140; // Recalcula y1

    // Desenha a marca
    tft.drawLine(x0, y0, x1, y1, TFT_BLACK); // Desenha uma linha preta representando a marca

    // Verifica se as labels devem ser desenhadas, com ajustes de posição
    if (i % 25 == 0) {
      x0 = sx * (M_SIZE*100 + tl + 10) + M_SIZE*120; // Posição x da label
      y0 = sy * (M_SIZE*100 + tl + 10) + M_SIZE*140; // Posição y da label
      switch (i / 25) {
        case -2: tft.drawCentreString("0", x0, y0 - 12, 2); break;    // Label "0" para -50 graus
        case -1: tft.drawCentreString("25", x0, y0 - 9, 2); break;   // Label "25" para -25 graus
        case 0: tft.drawCentreString("50", x0, y0 - 7, 2); break;    // Label "50" para 0 graus
        case 1: tft.drawCentreString("75", x0, y0 - 9, 2); break;    // Label "75" para 25 graus
        case 2: tft.drawCentreString("100", x0, y0 - 12, 2); break;  // Label "100" para 50 graus
      }
    }

    sx = cos((i + 5 - 90) * 0.0174532925); // Calcula o cosseno para o arco
    sy = sin((i + 5 - 90) * 0.0174532925); // Calcula o seno para o arco
    x0 = sx * M_SIZE*100 + M_SIZE*120; // Posição x para o arco
    y0 = sy * M_SIZE*100 + M_SIZE*140; // Posição y para o arco

    // Desenha o arco da escala
    if (i < 50) tft.drawLine(x0, y0, x1, y1, TFT_BLACK); // Desenha uma linha preta representando parte do arco
  }

  // Exibe a unidade "%DC" na parte superior
  tft.drawCentreString("%DC", M_SIZE*120, M_SIZE*70, 4); 
  tft.drawRect(5, 3, M_SIZE*230, M_SIZE*119, TFT_BLACK); // Desenha o contorno do medidor

  plotNeedle(0, 0); // Posiciona a agulha do medidor no valor 0
}

void plotNeedle(int value, byte ms_delay) {
  tft.setTextColor(TFT_CYAN, TFT_BLACK); // Define a cor do texto como preto
  char buf[8]; dtostrf(value, 4, 0, buf); // Converte o valor em string
  tft.drawRightString(buf, M_SIZE*40, M_SIZE*(119 - 20), 2); // Desenha o valor à direita

  if (value < -10) value = -10; // Limita o valor mínimo
  if (value > 110) value = 110; // Limita o valor máximo

  // Move a agulha até que o valor desejado seja alcançado
  while (!(value == old_analog)) {
    if (old_analog < value) old_analog++; // Incrementa o valor da agulha
    else old_analog--; // Decrementa o valor da agulha

    if (ms_delay == 0) old_analog = value; // Atualiza a agulha instantaneamente se ms_delay for 0

    float sdeg = map(old_analog, -10, 110, -150, -30); // Mapeia o valor para um ângulo entre -150 e -30 graus
    float sx = cos(sdeg * 0.0174532925); // Calcula o cosseno do ângulo
    float sy = sin(sdeg * 0.0174532925); // Calcula o seno do ângulo

    float tx = tan((sdeg + 90) * 0.0174532925); // Calcula o tangente para ajuste da base da agulha

    // Apaga a agulha antiga desenhando linhas pretas sobre ela
    tft.drawLine(M_SIZE*(120 + 20 * ltx - 1), M_SIZE*(140 - 20), osx - 1, osy, TFT_BLACK);
    tft.drawLine(M_SIZE*(120 + 20 * ltx), M_SIZE*(140 - 20), osx, osy, TFT_BLACK);
    tft.drawLine(M_SIZE*(120 + 20 * ltx + 1), M_SIZE*(140 - 20), osx + 1, osy, TFT_BLACK);

    // Redesenha a agulha em vermelho
    tft.setTextColor(TFT_CYAN); 
    tft.drawCentreString("%DC", M_SIZE*120, M_SIZE*70, 4); 

    // Armazena as coordenadas da nova agulha
    ltx = tx;
    osx = M_SIZE*(sx * 98 + 120);
    osy = M_SIZE*(sy * 98 + 140);

    // Desenha a nova agulha
    tft.drawLine(M_SIZE*(120 + 20 * ltx - 1), M_SIZE*(140 - 20), osx - 1, osy, TFT_RED);
    tft.drawLine(M_SIZE*(120 + 20 * ltx), M_SIZE*(140 - 20), osx, osy, TFT_MAGENTA);
    tft.drawLine(M_SIZE*(120 + 20 * ltx + 1), M_SIZE*(140 - 20), osx + 1, osy, TFT_RED);

    // Ajusta o atraso para suavizar o movimento
    if (abs(old_analog - value) < 10) ms_delay += ms_delay / 5;

    // Aguarda antes de atualizar novamente
    delay(ms_delay);
  }
}

void plotLinear(char *label, int x, int y) {
  int w = 36; // Define a largura do medidor linear
  tft.drawRect(x, y, w, 155, TFT_GREY); // Desenha o contorno do medidor linear
  tft.fillRect(x+2, y + 19, w-3, 155 - 38, TFT_WHITE); // Preenche a área interna do medidor com branco
  tft.setTextColor(TFT_CYAN, TFT_BLACK); // Define a cor do texto como ciano com fundo preto
  tft.drawCentreString(label, x + w / 2, y + 2, 2); // Desenha o rótulo no topo do medidor linear

  // Desenha marcas horizontais menores
  for (int i = 0; i < 110; i += 10) {
    tft.drawFastHLine(x + 20, y + 27 + i, 6, TFT_BLACK); // Marcas pequenas
  }

  // Desenha marcas horizontais maiores
  for (int i = 0; i < 110; i += 50) {
    tft.drawFastHLine(x + 20, y + 27 + i, 9, TFT_BLACK); // Marcas maiores
  }

  // Desenha triângulos vermelhos que representam o ponteiro na parte inferior e superior
  tft.fillTriangle(x+3, y + 127, x+3+16, y+127, x + 3, y + 127 - 5, TFT_RED); // Triângulo na parte inferior
  tft.fillTriangle(x+3, y + 127, x+3+16, y+127, x + 3, y + 127 + 5, TFT_RED); // Triângulo na parte superior

  // Adiciona um espaço de marcador (---) na parte inferior do medidor
  tft.drawCentreString("---", x + w / 2, y + 155 - 18, 2); // Placeholder para mostrar valores
}

void plotPointer(void) {
  int dy = 187; // Define a posição y base para os ponteiros
  byte pw = 16; // Define o comprimento dos ponteiros

  tft.setTextColor(TFT_GREEN, TFT_CYAN); // Define a cor do texto como verde com fundo preto

  // Move os 6 ponteiros um pixel em direção ao novo valor
  for (int i = 0; i < 6; i++) {
    char buf[8]; dtostrf(value[i], 4, 0, buf); // Converte o valor atual para string
    tft.drawRightString(buf, i * 40 + 36 - 5, 187 - 27 + 155 - 18, 2); // Desenha o valor à direita do medidor

    int dx = 3 + 40 * i; // Calcula a posição x do ponteiro baseado no índice
    if (value[i] < 0) value[i] = 0; // Limita o valor mínimo
    if (value[i] > 100) value[i] = 100; // Limita o valor máximo

    // Move o ponteiro até que o valor antigo iguale ao novo valor
    while (!(value[i] == old_value[i])) {
      dy = 187 + 100 - old_value[i]; // Calcula a posição y atual do ponteiro
      if (old_value[i] > value[i]) {
        tft.drawLine(dx, dy - 5, dx + pw, dy, TFT_WHITE); // Apaga o ponteiro antigo desenhando branco
        old_value[i]--; // Diminui o valor antigo
        tft.drawLine(dx, dy + 6, dx + pw, dy + 1, TFT_RED); // Desenha o novo ponteiro em vermelho
      } else {
        tft.drawLine(dx, dy + 5, dx + pw, dy, TFT_WHITE); // Apaga o ponteiro antigo desenhando branco
        old_value[i]++; // Aumenta o valor antigo
        tft.drawLine(dx, dy - 6, dx + pw, dy - 1, TFT_RED); // Desenha o novo ponteiro em vermelho
      }
    }
  }
}
