#include <LiquidCrystal.h> //Inclui a biblioteca do LCD

LiquidCrystal lcd(2,3,4,5,6,7);

int segundo,minuto, hora, dia, mes,ano;

unsigned long UtlTime;

void setup(){ 
  
  UtlTime=0;
  minuto=0; 
  hora=0; 
  dia=0; 
  mes=0;
  ano=0; 
  Serial.begin(9600);
  lcd.begin(16, 2); 
  lcd.setCursor(0,0); 
  lcd.print(" Data e hora ");
  lcd.setCursor(0,1); 
  lcd.print(" com Arduino");
  delay (2000); //Configura o minuto
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Minuto: ");
  Serial.print("\nEntre Minuto:");
  while(minuto==0)  { 
    if (Serial.available() > 0){
      minuto= Serial.parseInt();
     }} 
lcd.print(minuto);
delay(1000); //Configura a hora

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Hora: ");
Serial.print("\nEntre Hora:");
while(hora==0) {
if (Serial.available() > 0) {
hora= Serial.parseInt(); } }
lcd.print(hora);
delay(1000); //Configura o Dia

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Dia: ");
Serial.print("\nEntre Dia:");
while(dia==0) {
  if (Serial.available() > 0) {
    dia= Serial.parseInt();  } } 
    lcd.print(dia);
    delay(1000); //Configura o mês
    
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Mes: ");
Serial.print("\nEntre Mes:");
while(mes==0) { 
  if (Serial.available() > 0) { 
    mes= Serial.parseInt(); } }
    lcd.print(mes);
    delay(1000); //Configura o Ano

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Ano: "); 
Serial.print("\nEntre ano:"); 
while(ano==0) {
  if (Serial.available() > 0) {
    ano= Serial.parseInt(); } }
    lcd.print(ano); delay(1000);
    lcd.clear();}

void loop(){
  if(millis()-UtlTime<0) {
    UtlTime=millis(); }
    else {
      segundo=int((millis()-UtlTime)/1000); }
      if(segundo>59) {
        segundo=0;
        minuto++; 
        UtlTime=millis();
        if(minuto>59) {
          hora++; minuto=0; if(hora>23) { 
            dia++;
hora=0;
if(mes==1||mes==3||mes==5||mes==7||mes==8||mes==10||mes==12) { 
  if(dia>31) { dia=1; mes++; if(mes>12) {
    ano++; mes=1; } } }
    else if(mes==2) { 
      if(ano%400==0) {
        if(dia>29) { dia=1; mes++; }} 
        else if((ano%4==0)&&(ano%100!=0)) {
          if(dia>29) { dia=1; mes++; } }
          else { if(dia>28) {
            dia=1; mes++; } } }
            else { if(dia>30) { 
              dia=1; mes++; } } } } }
              Serial.print(dia);
              Serial.print("/");
              Serial.print(mes);
              Serial.print("/");
              Serial.print(ano);
              Serial.println();
              lcd.setCursor(0,0); 

lcd.print("Data ");
lcd.print(dia);
lcd.print("/"); 
lcd.print(mes); 
lcd.print("/"); 
lcd.print(ano); 
Serial.print(hora);
Serial.print(":"); 
Serial.print(minuto);
Serial.print(":"); 
Serial.print(segundo);
Serial.print("\n"); 
Serial.println(); 
lcd.setCursor(0,1);
lcd.print("Hora ");
lcd.print(hora); 
lcd.print(":"); 
lcd.print(minuto);
lcd.print(":"); 
lcd.print(segundo);}
