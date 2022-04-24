#define T1 3  //Baja Brazo
#define T2 2  //Giro angular garra (180°) 
#define T3 9  //Direccion radial
#define T4 8  //Cierre de garra
#define T5 7  //Tope estado final
#define T6 6  //Giro angular brazo (180°)

#define S1 A3  //Baja Brazo
#define S2 A1  //Giro angular garra (180°) 
#define S3 A0  //Direccion radial
#define S4 A2  //Cierre de garra
#define S5 A4  //Tope estado final

void setup() {

//Definiendo Teclas o Actuadores  
pinMode(T1,OUTPUT);
pinMode(T2,OUTPUT);
pinMode(T3,OUTPUT);
pinMode(T4,OUTPUT);
pinMode(T5,OUTPUT);
pinMode(T6,OUTPUT);
pinMode(T7,OUTPUT);
pinMode(T8,OUTPUT);

//Definiendo Sensores
pinMode(S1,INPUT);
pinMode(S2,INPUT);
pinMode(S3,INPUT);
pinMode(S4,INPUT);
pinMode(S5,INPUT);

//Seteando HOME 
//Paso 1 y 2
digitalWrite(T1,HIGH);
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH)) {digitalWrite(T6,HIGH);}

}

void loop() {
//Paso 3
if (digitalRead(S2,HIGH) && digitalRead(S5,HIGH)) {digitalWrite(T3,HIGH);}
delay(1000);
//Paso 4 
if (digitalRead(S2,HIGH) && digitalRead(S5,HIGH) && digitalRead(S3,HIGH)) {digitalWrite(T4,HIGH);}
delay(1000);
//Paso 5 
if (digitalRead(S2,HIGH) && digitalRead(S5,HIGH) && digitalRead(S3,HIGH)) {digitalWrite(T3,LOW);}
delay(1000);
//Paso 6
if (digitalRead(S2,HIGH) && digitalRead(S5,HIGH) ) {digitalWrite(T6,LOW);}
delay(1000);
//Paso 7
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH) ) {digitalWrite(T3,HIGH);}
delay(1000);
//Paso 8
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH) && digitalRead(S3,HIGH)) {digitalWrite(T4,LOW);}
delay(1000);
//Paso 9
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH) && digitalRead(S3,HIGH)) {digitalWrite(T3,LOW);}
delay(1000);
//Paso 10
delay(5000); //Espero a que termine de trabajar la fresa
//Paso 11
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH) ) {digitalWrite(T3,HIGH);}
delay(1000);
//Paso 12
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH) && digitalRead(S3,HIGH)) {digitalWrite(T4,HIGH);}
delay(1000);
//Paso 13
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH) && digitalRead(S3,HIGH)) {digitalWrite(T3,LOW);}
delay(1000);
//Paso 14
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH) ) {digitalWrite(T2,HIGH);}
delay(1000);
//Paso 15
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH) ) {digitalWrite(T3,HIGH);}
delay(1000);
//Paso 16
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH) && digitalRead(S3,HIGH)) {digitalWrite(T4,LOW);}
delay(1000);
//Paso 17
if (digitalRead(S1,HIGH) && digitalRead(S3,HIGH) && digitalRead(S5,HIGH)) {digitalWrite(T3,LOW);}
delay(1000);
//Paso 18
delay(5000); //Espero a que trabaje la fresa con la pieza dada vuelta
//Paso 19
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH) ) {digitalWrite(T3,HIGH);}
delay(1000);
//Paso 20
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH) && digitalRead(S3,HIGH)) {digitalWrite(T4,HIGH);}
delay(1000);
//Paso 21
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH) && digitalRead(S3,HIGH)) {digitalWrite(T3,LOW);}
delay(1000);
//Paso 22
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH) ) {digitalWrite(T5,HIGH);}
delay(1000);
//Paso 23
if (digitalRead(S1,HIGH) && digitalRead(S5,HIGH) && digitalRead(S4,HIGH)) {digitalWrite(T6,HIGH);}
delay(1000);
//Paso 24
if (digitalRead(S4,HIGH) && digitalRead(S5,HIGH)) {digitalWrite(T4,LOW);}
delay(1000);
//Paso 25
if (digitalRead(S2,HIGH) && digitalRead(S5,HIGH)) {digitalWrite(T5,LOW);}
delay(1000);
//Paso 26
//Termino en el estado HOME
}
