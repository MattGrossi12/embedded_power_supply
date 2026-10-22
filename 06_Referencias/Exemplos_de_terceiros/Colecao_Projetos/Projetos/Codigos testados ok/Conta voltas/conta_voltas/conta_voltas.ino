

//https://www.youtube.com/watch?v=eWRLXxrKpt8

int conta=0;


void setup(){
  Serial.begin(9600);
  pinMode(8,INPUT_PULLUP);
  pinMode (13, OUTPUT);
}

void loop() {
 
if (digitalRead(2) == HIGH)
{
  if (digitalRead(2) == LOW)
  {
    conta++;
    Serial.print(conta);
    delay (100);
    
    }
  }
if (conta ==5)
{
  digitalWrite(13, HIGH);
  }
if (conta==8)
{
  digitalWrite(13, LOW);
  
  
  }

  
}
