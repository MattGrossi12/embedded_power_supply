void setup() {
  // put your setup code here, to run once:
pinMode(10, OUTPUT);
pinMode(7,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

int estadobotao;
estadobotao = digitalRead(7);

digitalWrite(10, estadobotao);

delay(200);

}
