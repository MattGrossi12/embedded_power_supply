
//Mapeamento de hardware
#define   potVel   A0
#define   potDir   A1
#define   m1a      6
#define   m1b      7
#define   pwm      3

int leituraVel = 0, leituraDir = 0;

//Configuração inicial
void setup()
{
  //Entradas e saídas
   pinMode(potVel, INPUT);
   pinMode(potDir, INPUT);
   pinMode(m1a, OUTPUT);
   pinMode(m1b, OUTPUT);
   pinMode(pwm, OUTPUT);
   
   Serial.begin(9600);

}

void loop()
{
   leituraVel = analogRead(potVel) / 4;
   leituraDir = analogRead(potDir);
   
   
   

   if(leituraDir < 450)
   {
     analogWrite(pwm, leituraVel);    //Saída PWM para controle de velocidade
     
     Serial.println(leituraDir);
     
     digitalWrite(m1a, HIGH);
     digitalWrite(m1b, LOW);
   }
   
   if(leituraDir > 550)
   {
     analogWrite(pwm, leituraVel);    //Saída PWM para controle de velocidade
      Serial.println(leituraDir);
      
     digitalWrite(m1a, LOW);
     digitalWrite(m1b, HIGH);
   }
   
   else
   {
     analogWrite(pwm, leituraVel);    //Saída PWM para controle de velocidade
      Serial.println(leituraDir);
     digitalWrite(m1a, LOW);
     digitalWrite(m1b, LOW);
   
   }


} //end loop









