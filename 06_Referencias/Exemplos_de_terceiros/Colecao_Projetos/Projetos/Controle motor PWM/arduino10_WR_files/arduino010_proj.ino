
 #define velocidade A0
 #define direcao    A1
 #define m1a        6
 #define m1b        7
 #define pwm        3

int pwmRead, dirRead;

void setup() 
{
  pinMode(velocidade, INPUT);
  pinMode(direcao, INPUT);
  pinMode(m1a, OUTPUT);
  pinMode(m1b, OUTPUT);
  pinMode(pwm, OUTPUT); 
  
 Serial.begin(9600);
}

void loop() 
{
  pwmRead = analogRead(velocidade)/4;
  dirRead = analogRead(direcao);
  
  
  
  if(dirRead < 450)
  {
    analogWrite(pwm,pwmRead);
  Serial.println(dirRead);
    digitalWrite(m1a, HIGH);
    digitalWrite(m1b, LOW);
  }
  if(dirRead > 550)
  {
    analogWrite(pwm,pwmRead);
  Serial.println(dirRead);
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, HIGH);
  }
  
  else
  {
    analogWrite(pwm,pwmRead);
  Serial.println(dirRead);
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, LOW);
  }
  
  //delay(100);
  

}
