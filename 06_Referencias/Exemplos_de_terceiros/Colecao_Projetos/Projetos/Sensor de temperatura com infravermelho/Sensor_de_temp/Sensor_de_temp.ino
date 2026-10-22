// --- Leitura em Celsius ---
 
void setup()
 
{
 
Serial.begin(9600);             //Inicializa comunicação serial em 9600 de baud rate
 
therm.begin();                  //Inicializa sensor de temperatura infravermelho
 
therm.setUnit(TEMP_C);          //Seleciona temperatura em Celsius
 
}

// ou --- Leitura em Fahrenheit ---
 
void setup()
 
{
 
Serial.begin(9600);             //Inicializa comunicação serial em 9600 de baud rate
 
therm.begin();                  //Inicializa sensor de temperatura infravermelho
 
therm.setUnit(TEMP_F);          //Seleciona temperatura em Fahrenheit
 

//ou  --- Leitura em Kelvin ---
 
void setup()
 
{
 
Serial.begin(9600);             //Inicializa comunicação serial em 9600 de baud rate
 
therm.begin();                  //Inicializa sensor de temperatura infravermelho
 
therm.setUnit(TEMP_K);          //Seleciona temperatura em Kelvin
 
}
