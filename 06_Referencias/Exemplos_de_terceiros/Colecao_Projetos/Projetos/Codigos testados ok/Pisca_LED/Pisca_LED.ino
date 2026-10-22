void setup() {
  // put your setup code here, to run once:

pinMode(10, OUTPUT);
pinMode(9, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:

int intervalo1Pisca;
int intervalo2Pisca;

intervalo1Pisca = 200;
intervalo2Pisca = 200;

digitalWrite(10, HIGH);
digitalWrite(9, LOW);
delay(intervalo1Pisca);

digitalWrite(10, LOW);
digitalWrite(9, HIGH);
delay(intervalo2Pisca);

}
