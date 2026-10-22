//inclui biblioteca do sensor

#include <SensorNivel.h>

//inclui biblioteca de saida
#include <Saida.h>

//cria um objeto do tipo sensor
SensorNivel Sensor;

//cria um objeto do tipo saída
Saida Bomba(10);

//configura arduino
void setup()
{
}

//loop principal

void loop()
{
 switch(Sensor.getNivel())
 {
   case NIVEL_MIN:
     Bomba.liga();
   break;

   case NIVEL_MED:
   break;

   case NIVEL_MAX:
     Bomba.desliga();
   break;
 }
}
