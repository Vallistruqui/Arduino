//Control de MOTORES recibiendo como parámetro el ángulo que deben girar cada articulación para llegar al 
//punto del espacio asignado.
//------------------------------------------------------------------------------------------------------------------------

#define PINZA_PIN 5   
#define MUNECA_ROLL_PIN 10
#define MUNECA_PITCH_PIN 9          
#define CODO_PIN 11

#define MotorPin_hombro  7          
#define MotorDir_hombro  6   
#define MotorEnable_hombro 8 

#define MotorPin_cintura  3          
#define MotorDir_cintura  4   
#define MotorEnable_cintura 2 

//------------------------------------------------------------------------------------------------------------------------

#define LIM_DER_PITCH 83               //Limite izquierdo mayor a limite derecho
#define LIM_IZQ_PITCH 162 
#define LIM_DER_ROLL 80                //Limite izquierdo mayor a limite derecho
#define LIM_IZQ_ROLL 120 
#define LIM_DER_PINZA 0                //Limite izquierdo mayor a limite derecho
#define LIM_IZQ_PINZA 0 
#define LIM_DER_CODO 17                //Limite izquierdo mayor a limite derecho
#define LIM_IZQ_CODO 130

#define del_time_hombro  2000  
#define del_time_cintura  800
#define pasos_hombro 752               //50° son 470 pasos  -  1° = 9,4 pasos
#define pasos_cintura 840              //90° son 840 pasos  -  1° = 9,33 pasos

//Posicion inicial de las articulaciones
//Angulos respecto de las referencias de cada motor
#define ORIG_PITCH 162
#define ORIG_ROLL 100
#define ORIG_CODO 17
#define ORIG_HOMBRO 0
#define ORIG_CINTURA 0
#define ORIG_PINZA 0

int servopos_codo = ORIG_CODO;
int servopos_pinza = ORIG_PINZA;
int servopos_pitch = ORIG_PITCH;
int servopos_roll = ORIG_ROLL;
int pappos_cintura = ORIG_CINTURA;
int pappos_hombro = ORIG_HOMBRO; 

//tiempo que hay que esperar entre grado y grado. Unidad = [miliseg]
int step_time_pinza = 20;
int step_time_roll = 10;
int step_time_pitch = 20;
int step_time_codo = 20;

//------------------------------------------------------------------------------------------------------------------------

#include <Servo.h>

Servo pinza;
Servo muneca_roll;            //Entre 20 y 180, 100 pos inicial, 100 cero de ref
Servo muneca_pitch;           //Entre 162 y 70, 162 pos inicial, 83 cero de ref
Servo codo;                   //Entre 17 y 130, 17 pos inicial, 93 cero de ref


int angRotCadera;
int anguloCadera;
int posCaderaActual;
int pasosCadera;

int angRotHombro;
int anguloHombro;
int posHombroActual;
int pasosHombro;

int posCodoActual;
int anguloCodo;

int posPitchActual;
int anguloPitch;

int posRollActual;
int anguloRoll;

int angulos[3];
int comunicacionMLab;

bool caderaBool;
bool hombroBool;

int posPinzaActual;
int anguloPinza;

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup() 
{
  //Recibimos ángulos del Matlab
//  angulos[] = [2, 4, 6];
  
  //anguloCadera = angulos[1];
  //anguloHombro = angulos[2];
//  anguloCodo = angulos[3];
  //anguloPitch = angulos[4];
  //anguloRoll = angulos[5];

 //Enviamos señal a los motores para que realicen los movimeintos
  //servo.attach(PIN); ///////////////////ADVERTENCIA//////////////////
  //servo.write(ANGULO);//////////////////ADVERTENCIA//////////////////

  
  pinza.attach(PINZA_PIN);   //Defino la pata que controlará el motor   (450,2175)
  muneca_roll.attach(MUNECA_ROLL_PIN);
  muneca_pitch.attach(MUNECA_PITCH_PIN);
  codo.attach(CODO_PIN);

  Serial.begin(115200);
  servopos_pitch = constrain(servopos_pitch,70,162);
  servopos_roll = constrain(servopos_roll,20,180);
  servopos_codo = constrain(servopos_codo,17,130);
  servopos_pinza = constrain(servopos_pinza,0,180);
    
  pinMode(MotorPin_hombro,OUTPUT);
  pinMode(MotorDir_hombro,OUTPUT);
  pinMode(MotorEnable_hombro,OUTPUT);
  pinMode(MotorPin_cintura,OUTPUT);
  pinMode(MotorDir_cintura,OUTPUT);
  pinMode(MotorEnable_cintura,OUTPUT);
  
  pinMode(PINZA_PIN,OUTPUT);
  pinMode(MUNECA_PITCH_PIN,OUTPUT);
  pinMode(MUNECA_ROLL_PIN,OUTPUT);
  pinMode(CODO_PIN,OUTPUT);
  pinMode(A1,OUTPUT);

//Rele que conecta la energia al inicializar el codigo
  digitalWrite(A1,HIGH);
  
  muneca_pitch.write(ORIG_PITCH);
  muneca_roll.write(ORIG_ROLL);
  pinza.write(ORIG_CODO);
  codo.write(ORIG_CODO);

  digitalWrite(MotorEnable_hombro,LOW);
  digitalWrite(MotorEnable_cintura,LOW);
  delay(5000);
   
posHombroActual = 140;
posCaderaActual = 0;
posCodoActual = 17;
posPitchActual = 162;
posRollActual = 100;
posPinzaActual = 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

void loop() 
{
anguloPinza = 0;
anguloCadera = 0;
anguloHombro = 140; //acordar que es 140
anguloCodo = 17;
anguloPitch = 162;
anguloRoll = 100;

  // ángulo inicio Hombro = 140
  // ángulo inicio Codo = 17
  // ángulo inicio Pitch = 160

  //Recibimos ángulos del Matlab  
  if(Serial.available()>0)
  { 
      anguloCadera = Serial.read();
      delay(10);
      anguloHombro = Serial.read();
      delay(10);
      anguloCodo = Serial.read();
      delay(10);
      anguloPitch = Serial.read();
      delay(10);
      anguloRoll = Serial.read();
      delay(10);
      anguloPinza = Serial.read();
      delay(10);
   }   
/////////////////////////////////////////////////////////////////////////MOVIMIENTO DE CADERA//////////////////////////////////////////////////////////////////////////
 
//Definimos cantidad de pasos necesarios (teniendo en cuenta los limites estructurales)
  
  angRotCadera = anguloCadera - posCaderaActual;
  
  if(angRotCadera<0){
  //muevo el brazo para la derecha
    digitalWrite(MotorDir_cintura,HIGH);
    caderaBool = true;
  }
  else {
    digitalWrite(MotorDir_cintura,LOW);  
    caderaBool = false;
  }
  
  angRotCadera = abs(angRotCadera);
  pasosCadera = angRotCadera*9.33;

//Ejecucion de la Instruccion
   for (int i = 0; i <= pasosCadera; i++) {
  digitalWrite(MotorPin_cintura, HIGH);
  delayMicroseconds(del_time_cintura);
  digitalWrite(MotorPin_cintura, LOW);
  delayMicroseconds(del_time_cintura);
}
delay(1000);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////MOVIMIENTO DE HOMBRO//////////////////////////////////////////////////////////////////////////
 
//Definimos cantidad de pasos necesarios (teniendo en cuenta los limites estructurales)
  
  angRotHombro = anguloHombro - posHombroActual;
  
  if(angRotHombro<0){
  //muevo el brazo para la derecha
    digitalWrite(MotorDir_hombro,HIGH);
    hombroBool = true;
  }
  else {
    digitalWrite(MotorDir_hombro,LOW);
    hombroBool = false;
  }
  
  angRotHombro = abs(angRotHombro);
  pasosHombro = angRotHombro*9.4;

//Ejecucion de la Instruccion
   for (int i = 0; i <= pasosHombro; i++) {
  digitalWrite(MotorPin_hombro, HIGH);
  delayMicroseconds(del_time_hombro);
  digitalWrite(MotorPin_hombro, LOW);
  delayMicroseconds(del_time_hombro);
}

delay(1000);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////MOVIMIENTO DE CODO//////////////////////////////////////////////////////////////////////////

if( anguloCodo<= LIM_IZQ_CODO && anguloCodo >= LIM_DER_CODO){
if(posCodoActual >= anguloCodo){
  //muevo el brazo para la derecha
 for (int i = posCodoActual; posCodoActual != anguloCodo; i--) {
  codo.write(i);
  posCodoActual = i;
  delay(step_time_codo);
  }
  }
  else {
     for (int i = posCodoActual; posCodoActual != anguloCodo; i++) {
  codo.write(i);
  posCodoActual = i;
  delay(step_time_codo);
  }
  }
  delay(1000);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////MOVIMIENTO DE PITCH//////////////////////////////////////////////////////////////////////////
if( anguloPitch<= LIM_IZQ_PITCH && anguloPitch >= LIM_DER_PITCH){
if(posPitchActual >= anguloPitch){
  //muevo el brazo para la derecha
 for (int i = posPitchActual; posPitchActual != anguloPitch; i--) {
  muneca_pitch.write(i);
  posPitchActual = i;
  delay(step_time_pitch);
  }
  }
  else {
     for (int i = posPitchActual; posPitchActual != anguloPitch; i++) {
  muneca_pitch.write(i);
  posPitchActual = i;
  delay(step_time_pitch);
  }
  }
  delay(1000);

}
  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////MOVIMIENTO DE PINZA//////////////////////////////////////////////////////////////////////////

if( anguloRoll<= LIM_IZQ_ROLL && anguloRoll >= LIM_DER_ROLL){
if(posRollActual >= anguloRoll){
  //muevo el brazo para la derecha
 for (int i = posRollActual; posRollActual != anguloRoll; i--) {
  muneca_roll.write(i);
  posRollActual = i;
  delay(step_time_roll);
  }
  }
  else {
     for (int i = posRollActual; posRollActual != anguloRoll; i++) {
  muneca_roll.write(i);
  posRollActual = i;
  delay(step_time_roll);
  }
  }
  delay(1000);
  
}
  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////APERTURA DE PINZA//////////////////////////////////////////////////////////////////////////

pinza.write(anguloPinza);
delay(1000);
  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pinza.write(0);
if(posRollActual >= ORIG_ROLL){
  //muevo el brazo para la derecha
 for (int i = posRollActual; posRollActual != ORIG_ROLL; i--) {
  muneca_roll.write(i);
  posRollActual = i;
  delay(step_time_roll);
  }
  }
  else {
     for (int i = posRollActual; posRollActual != ORIG_ROLL; i++) {
  muneca_roll.write(i);
  posRollActual = i;
  delay(step_time_roll);
  }
  }
delay(500);
  
  for (int i = posPitchActual; posPitchActual != ORIG_PITCH; i++) {
  muneca_pitch.write(i);
  posPitchActual = i;
  delay(step_time_pitch);
  }
delay(500);
  for (int i = posCodoActual; posCodoActual != ORIG_CODO; i--) {
  codo.write(i);
  posCodoActual = i;
  delay(step_time_codo);
  }
delay(500);
    if(hombroBool == true){
  //muevo el brazo para la derecha
    digitalWrite(MotorDir_hombro,LOW);
  }
  else {
    digitalWrite(MotorDir_hombro,HIGH);
  }
     for (int i = 0; i <= pasosHombro; i++) {
  digitalWrite(MotorPin_hombro, HIGH);
  delayMicroseconds(del_time_hombro);
  digitalWrite(MotorPin_hombro, LOW);
  delayMicroseconds(del_time_hombro);
}
 
delay(500);
  if(caderaBool == true){
  //muevo el brazo para la derecha
    digitalWrite(MotorDir_cintura,LOW);
  }
  else {
    digitalWrite(MotorDir_cintura,HIGH);  
  }
     for (int i = 0; i <= pasosCadera; i++) {
  digitalWrite(MotorPin_cintura, HIGH);
  delayMicroseconds(del_time_cintura);
  digitalWrite(MotorPin_cintura, LOW);
  delayMicroseconds(del_time_cintura);
}
delay(2000);



/////////////////////////////////////////////////////////////////////////SERIAL PRINTS//////////////////////////////////////////////////////////////////////////

Serial.print ("Pasos hombro: ");
Serial.print (pasosHombro);
Serial.print ("         Angulo hombro: ");
Serial.print (angRotHombro);

Serial.print ("         Pasos cadera: ");
Serial.print (pasosCadera);
Serial.print ("         Angulo cadera: ");
Serial.print (angRotCadera);
}
