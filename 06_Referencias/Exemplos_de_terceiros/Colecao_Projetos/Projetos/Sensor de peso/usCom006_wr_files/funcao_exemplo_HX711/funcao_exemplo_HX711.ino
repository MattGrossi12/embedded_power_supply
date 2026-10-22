/*
      Teste de função em C para funcionamento do CI HX711
      
      Conversor AD para célula de carga


      Eng. Wagner Rambo
      Outubro de 2016
*/

// --- Mapeamento de Hardware ---
#define  ADDO  7    //Data Out
#define  ADSK  6    //SCK


// --- Protótipo das Funções Auxiliares ---
unsigned long ReadCount(); //conversão AD do HX711


// --- Variáveis Globais ---
unsigned long gramas = 0;
String peso_digitado;
int peso_desejado;
boolean stringCompleta = false;
unsigned long convert;


// --- Configurações Iniciais ---
void setup()
{
   pinMode(ADDO, INPUT_PULLUP);   //entrada para receber os dados
   pinMode(ADSK, OUTPUT);         //saída para SCK

   pinMode(13, OUTPUT);
   
   
   Serial.begin(9600);

   
  Serial.println("Digite o peso desejado:");

} //end setup


// --- Loop Infinito ---
void loop()
{
  
  convert = ReadCount()-8652000;

  gramas = convert/1000;


  
  serialEvent();

  if (stringCompleta) {
    peso_desejado = peso_digitado.toInt();

    digitalWrite(13, HIGH);
    
    Serial.print("O peso desejado: ");
    Serial.println(peso_desejado, DEC);
    stringCompleta = false;
    peso_digitado = "";
  }

  
  if (peso_desejado < gramas ) {
    digitalWrite(13, LOW);
  }


} //end loop


// --- Funções ---
unsigned long ReadCount()
{
  unsigned long Count = 0;
  unsigned char i;
  
  digitalWrite(ADSK, LOW);
  
  while(digitalRead(ADDO));
  
  for(i=0;i<24;i++)
  {
     digitalWrite(ADSK, HIGH);
     Count = Count << 1;
     digitalWrite(ADSK, LOW);
     if(digitalRead(ADDO)) Count++;
  
  } //end for
  
  digitalWrite(ADSK, HIGH);
  Count = Count^0x800000;
  digitalWrite(ADSK, LOW);
  
  return(Count);


} //end ReadCount

void serialEvent() {
    
   if (Serial.available())
   {
    char inChar = (char)Serial.read();

    peso_digitado += inChar;

    if (inChar == '\n') {
      stringCompleta = true;
    }

    
   }

  
}









