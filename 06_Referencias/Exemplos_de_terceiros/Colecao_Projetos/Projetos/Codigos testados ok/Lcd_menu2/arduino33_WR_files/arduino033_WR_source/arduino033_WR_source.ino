/*
    Curso de Arduino Aula 033 - WR Kits Channel
    
    Lendo Rotary Encoder
    
    Autor: Eng. Wagner Rambo   Setembro de 2015
    
    www.wrkits.com.br | facebook.com/wrkits | youtube.com/user/canalwrkits

*/


// --- Mapeamento de Hardware ---
/* Rotary encoder read example */
#define ENC_A 8
#define ENC_B 9
#define ENC_PORT PINB
void setup()
{
/* Setup encoder pins as inputs */
pinMode(ENC_A, INPUT);
digitalWrite(ENC_A, HIGH);
pinMode(ENC_B, INPUT);
digitalWrite(ENC_B, HIGH);
Serial.begin (9600);
Serial.println("Start");

}

void loop()
{
static unsigned char counter = 0; //this variable will be changed by encoder input
char tmpdata;
/**/
tmpdata = read_encoder();
if( tmpdata ) {
Serial.print("Counter value: ");
Serial.println(counter, DEC);

counter += tmpdata;

}

}

/* returns change in encoder state (-1,0,1) */
char read_encoder()
{
  static char enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  static unsigned char old_AB = 0;
  /**/
  old_AB <<= 2;                   //remember previous state
  old_AB |= ( ENC_PORT & 0x03 );  //add current state
  return ( enc_states[( old_AB & 0x0f )]);
}
