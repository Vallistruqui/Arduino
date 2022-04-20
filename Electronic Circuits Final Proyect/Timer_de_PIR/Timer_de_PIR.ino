//https://articulo.mercadolibre.com.ar/MLA-713094295-sensor-movimiento-hc-sr501-pir-infrarrojo-arduino-mona-_JM
int lectura = 7;
int PIR = 0;
unsigned long t1 = 0
unsigned long t2 = 0
unsigned long tiempo = 0
int velocidad = 0
int velocidadMAX = 15
void setup() {

pinMode(lectura,INPUT);
}

void loop() {
  PIR = digitalRead(lectura);

  if(PIR==RISING) {t1 = micros()}
  if(PIR==FALLING) {t2 = micros()}
  tiempo = (t2 - t1) ;
  velocidad = (Longitud /10) / ((t2-t1)*1000000); // Longitud esta en milimetros y tiempo esta en microsegundos,  por lo tanto al hacer las siguientes operaciones queda en cm/s
  if (velocidad > velocidadMAX ){t2 = micros()}
}
