//Prueba de MOTORES de JARVIS

//------------------------------------------------------------------------------------------------------------------------

#define PINZA_PIN 12   // 8
#define MUNECA_ROLL_PIN 10
#define MUNECA_PITCH_PIN 9          
#define CODO_PIN 11

#define MotorPin_hombro  7 //7 para hombro - 3 para cintura         
#define MotorDir_hombro  6  //6 para hombro - 4 para cintura 
#define MotorEnable_hombro 5 //5 para hombro - 2 para cintura

#define MotorPin_cintura  3 //7 para hombro - 3 para cintura         
#define MotorDir_cintura  4  //6 para hombro - 4 para cintura 
#define MotorEnable_cintura 2 //5 para hombro - 2 para cintura

//------------------------------------------------------------------------------------------------------------------------

#define STEP_TIME_REG  15
#define STEP_TIME_BEG  35

#define LIM_DER_PITCH 83               //Limite izquierdo mayor a limite derecho
#define LIM_IZQ_PITCH 162 
#define LIM_DER_ROLL 80                //Limite izquierdo mayor a limite derecho
#define LIM_IZQ_ROLL 120 
#define LIM_DER_PINZA 0                //Limite izquierdo mayor a limite derecho
#define LIM_IZQ_PINZA 0 
#define LIM_DER_CODO 17                //Limite izquierdo mayor a limite derecho
#define LIM_IZQ_CODO 130

#define del_time_hombro  2000  
#define del_time_cintura  1000
#define pasos_hombro 752               //50° son 470 pasos
#define pasos_cintura 840              //90° son 840 pasos

 
#define ORIG_PITCH 162
#define ORIG_ROLL 100
#define ORIG_CODO 17

#define ACEL 1  

int servopos_codo = LIM_DER_CODO;
int servopos_pinza;
int servopos_pitch = LIM_IZQ_PITCH;
int servopos_roll = LIM_IZQ_ROLL;
int cont = 0;
char ORDEN = 0;


int servopos_final = 0;

int step_time_pinza = 20;
int step_time_roll = 10;
int step_time_pitch = 20;
int step_time_codo = 30;

//int delta_vel = STEP_TIME_BEG - STEP_TIME_REG;
//int delta_step = LIM_IZQ - LIM_DER;  

//------------------------------------------------------------------------------------------------------------------------

#include <Servo.h>

Servo pinza;
Servo muneca_roll;            //Entre 20 y 180, 100 pos inicial
Servo muneca_pitch;           //Entre 162 y 70, 162 pos inicial
Servo codo;                   //Entre 17 y 50, 17 pos inicial

void setup() 
{
  pinza.attach(PINZA_PIN);   //Defino la pata que controlará el motor   (450,2175)
  muneca_roll.attach(MUNECA_ROLL_PIN);
  muneca_pitch.attach(MUNECA_PITCH_PIN);
  codo.attach(CODO_PIN);

  Serial.begin(9600);
  servopos_pitch = constrain(servopos_pitch,0,180);
  servopos_roll = constrain(servopos_roll,0,180);
  servopos_codo = constrain(servopos_codo,0,180);
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

  digitalWrite(PINZA_PIN,LOW);
  digitalWrite(A1,HIGH);
  
  muneca_pitch.write(ORIG_PITCH);
  muneca_roll.write(ORIG_ROLL);
  pinza.write(0);
  codo.write(ORIG_CODO);

  digitalWrite(MotorEnable_hombro,LOW);
  digitalWrite(MotorEnable_cintura,LOW);
  
  delay(1000);
   
}

void loop() 
{
/*  if(Serial.available()>0)
  {
    ORDEN = Serial.read();
    
    if(ORDEN == 'A') //Inclincación de muñeca
    { 
      while(servopos_pitch > LIM_DER_PITCH)
     {
      muneca_pitch.write(servopos_pitch);
      //Serial.println(servopos_pitch);
      servopos_pitch = servopos_pitch - 1;
      delay(step_time_pitch);
     }
     
     delay(5000);
        
     while(servopos_pitch <= LIM_IZQ_PITCH)
     {
      muneca_pitch.write(servopos_pitch);
      //Serial.println(servopos_pitch);
      servopos_pitch = servopos_pitch + 1;
      delay(step_time_pitch);
     }

     delay(1000);
    }
    

    if(ORDEN == 'B') //Movimiento de codo
    {
      while(servopos_codo < LIM_IZQ_CODO)
      {
        codo.write(servopos_codo);
        //Serial.println(servopos_pitch);
        servopos_codo = servopos_codo + 1;
        delay(step_time_codo);
      }
   
      delay(5000);
      
      while(servopos_codo >= LIM_DER_CODO)
      {
        codo.write(servopos_codo);
        //Serial.println(servopos_pitch);
        servopos_codo = servopos_codo - 1;
        delay(step_time_codo);
      }

      delay(1000);
    }

    if(ORDEN == 'C') //Rotacion de muñeca
    {
      while(servopos_roll < LIM_IZQ_ROLL)
      {
        muneca_roll.write(servopos_roll);
        //Serial.println(servopos_pitch);
        servopos_roll = servopos_roll + 1;
        delay(step_time_roll);
      }
   
      delay(1000);
      
      while(servopos_roll >= LIM_DER_ROLL)
      {
        muneca_roll.write(servopos_roll);
        //Serial.println(servopos_pitch);
        servopos_roll = servopos_roll - 1;
        delay(step_time_roll);
      }   
     
      delay(1000);

      while(servopos_roll < ORIG_ROLL)
      {
        muneca_roll.write(servopos_roll);
        //Serial.println(servopos_pitch);
        servopos_roll = servopos_roll + 1;
        delay(step_time_roll);
      }   
     
      delay(1000);
    }
    if(ORDEN == 'D') // Movimiento de cintura
      {
        digitalWrite(MotorDir_cintura, HIGH);
      
        while(cont < pasos_cintura)
        {
          digitalWrite(MotorPin_cintura, HIGH);
          delayMicroseconds(del_time_cintura);
          digitalWrite(MotorPin_cintura, LOW);
          delayMicroseconds(del_time_cintura);
          
          cont ++;
        }
    
        delay(5000);
    
        cont = 0;
    
        digitalWrite(MotorDir_cintura, LOW);
    
        while(cont < pasos_cintura)
        {
          digitalWrite(MotorPin_cintura, HIGH);
          delayMicroseconds(del_time_cintura);
          digitalWrite(MotorPin_cintura, LOW);
          delayMicroseconds(del_time_cintura);
    
          cont ++;
        }
    
        cont = 0;
      } 
      else if(ORDEN == 'E')  //Movimiento de hombro
      {*/
        digitalWrite(MotorDir_hombro, HIGH);
      
        while(cont < pasos_hombro)
        {
          digitalWrite(MotorPin_hombro, HIGH);
          delayMicroseconds(del_time_hombro);
          digitalWrite(MotorPin_hombro, LOW);
          delayMicroseconds(del_time_hombro);
            
          cont ++;
        }
      
        delay(5000);
      
        cont = 0;
      
        digitalWrite(MotorDir_hombro, LOW);
      
        while(cont < pasos_hombro)
        {
          digitalWrite(MotorPin_hombro, HIGH);
          delayMicroseconds(del_time_hombro);
          digitalWrite(MotorPin_hombro, LOW);
          delayMicroseconds(del_time_hombro);
        
          cont ++;
        }
        cont = 0;
      }
     
  //}
//}
