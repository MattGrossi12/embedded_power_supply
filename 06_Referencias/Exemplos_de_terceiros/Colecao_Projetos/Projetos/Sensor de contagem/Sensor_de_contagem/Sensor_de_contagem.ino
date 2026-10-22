int sensor = 2;
int Nvoltas = 0;
  
void setup()
{
 Serial.begin(9600);
 Serial.println("Conta voltas"); 
 Serial.println("UsinaInfo"); 
}
  
  
void loop()
{
 if ( digitalRead(2)==1 )
 { 
 Nvoltas++;
 Serial.print(Nvoltas);
 }
}
