#include <Stepper.h>
const int stepsPerRevolution = 200;                                              // Tiene 200 pasos porque cada paso tiene 1.8°*200 = 360°
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

                                                                                 // Defino la conexion de todos los pines de la huerta
//#define humedadsuelo_1 A0
// #define humedadsuelo_2 A1
#define humedadsuelo_3 A2
#define humedadsuelo_4 A3
#define humedadsuelo_5 A4
#define humedadsuelo_6 A5

#define relay 5
int water_pump_status = 0;

#define light_sensor A0 
int Lvalue;
float light_level;
int open_status = 0;
int stepCount = 0;                                                               // number of steps the motor has taken
int stepper_delay = 10;

void setup() {
                                                                                 // Defino si los pines son INPUT (vamos a usar ese pin para un sensor) 
  pinMode(2,OUTPUT);                                                             // for EN1
  digitalWrite(2,HIGH);                                                          // enable EN1
  pinMode(3,OUTPUT);                                                             // for EN1
  digitalWrite(3,HIGH);                                                          // enable EN2
                                                                                 // o OUTPUT (vamos a usar ese pin para un actuador)  
  //  pinMode(humedadsuelo_1, INPUT);                                            //Sensor de Humedad de Suelo
  //  pinMode(humedadsuelo_2, INPUT);                                             //Sensor de Humedad de Suelo
  pinMode(humedadsuelo_3, INPUT);                                                //Sensor de Humedad de Suelo
  pinMode(humedadsuelo_4, INPUT);                                                //Sensor de Humedad de Suelo
  pinMode(humedadsuelo_5, INPUT);                                                //Sensor de Humedad de Suelo
  pinMode(humedadsuelo_6, INPUT);                                                //Sensor de Humedad de Suelo

  pinMode(relay,OUTPUT);                                                         //Rele que prende y apaga la bomba de agua 
  
  pinMode(light_sensor, INPUT);                                                  //Sensor de Luz  
  Serial.begin(9600);
}

void loop() {                                                                    //loop principal que se repite de manera infinita

Lvalue = analogRead(light_sensor);                                               //Lectura de la luz ambiente
light_level = map(Lvalue,0, 1024, 0.00, 100.00);                                 //Mapeo de esos valores a intervalo entre 0-100

if (light_level < 10 && open_status == 1)                                        //Hacer girar -153° el motor paso a paso pa que cierre la huerta cuando se haga de noche
{ 

while (stepCount > 0) {                                                          //Counter-Clockwise direction
    
//Serial.println("4ta combinacion");
digitalWrite(8, LOW);
digitalWrite(9, LOW);
digitalWrite(10, LOW);
digitalWrite(11, HIGH);
stepCount-- ;
delay(stepper_delay);

//Serial.println("3ra combinacion");
digitalWrite(8, LOW);
digitalWrite(9, LOW);
digitalWrite(10, HIGH);
digitalWrite(11, LOW);
stepCount-- ;
delay(stepper_delay);

//Serial.println("2da combinacion");
digitalWrite(8, HIGH);
digitalWrite(9, LOW);
digitalWrite(10, LOW);
digitalWrite(11, LOW);
stepCount-- ;
delay(stepper_delay);

//Serial.println("1ra combinacion");
digitalWrite(8, LOW);
digitalWrite(9, HIGH);
digitalWrite(10, LOW);
digitalWrite(11, LOW);
stepCount-- ;
delay(stepper_delay);

//Serial.println(stepCount);
}                                                                                 
                                                                                  
open_status = 0;

}
else if (light_level > 30 && open_status == 0)                                       //Hacer girar 153° el motor paso a paso para abrirla cuando arranca el dia 
{

while (stepCount < 85) {                                                          //Clockwise direction
//Serial.println("1ra combinacion");
digitalWrite(8, LOW);
digitalWrite(9, HIGH);
digitalWrite(10, LOW);
digitalWrite(11, LOW);
stepCount++ ;
delay(stepper_delay);

//Serial.println("2da combinacion");
digitalWrite(8, HIGH);
digitalWrite(9, LOW);
digitalWrite(10, LOW);
digitalWrite(11, LOW);
stepCount++ ;
delay(stepper_delay);
    
//Serial.println("3ra combinacion");
digitalWrite(8, LOW);
digitalWrite(9, LOW);
digitalWrite(10, HIGH);
digitalWrite(11, LOW);
stepCount++ ;
delay(stepper_delay);

//Serial.println("4ta combinacion");
digitalWrite(8, LOW);
digitalWrite(9, LOW);
digitalWrite(10, LOW);
digitalWrite(11, HIGH);
stepCount++ ;
delay(stepper_delay);

//Serial.println(stepCount);
}

open_status = 1;
}

// int  Humedad1 = analogRead(humedadsuelo_1);                                      // Lecturas de Humedad
// int  Humedad2 = analogRead(humedadsuelo_2);
int  Humedad3 = analogRead(humedadsuelo_3);
int  Humedad4 = analogRead(humedadsuelo_4);
int  Humedad5 = analogRead(humedadsuelo_5);
int  Humedad6 = analogRead(humedadsuelo_6);

if (/*Humedad1 <300|| Humedad2 <300||*/ Humedad3 <400||                              // Si alguno de los sensores esta humedo apagar la bomba
    Humedad4 <400|| Humedad5 <400|| Humedad6 <400)
{
  digitalWrite(relay, HIGH);
  water_pump_status = 0;
}
else if (/*Humedad1 >300&& Humedad2 >300 &&*/ Humedad3 >400 &&                         // Si todos los sensores estan secos, prender la bomba
         Humedad4 >400 && Humedad5 >400 && Humedad6 >400)
{
  digitalWrite(relay, LOW);
  water_pump_status = 1;
}

//  Serial.print("Tubo1: ");
//  Serial.println(Humedad1);
//  Serial.print("Tubo2: ");
//  Serial.println(Humedad2);
//  Serial.print("Tubo1: ");
//  Serial.println(Humedad3);
//  Serial.print("Tubo2: ");
//  Serial.println(Humedad4);
//  Serial.print("Tubo3: ");
//  Serial.println(Humedad5);
//  Serial.print("Tubo4: ");
//  Serial.println(Humedad6);
//  
//  Serial.println("");
//  
//  Serial.print("water_pump_status (0 OFF, 1 ON):  ");
//  Serial.println(water_pump_status);
//  Serial.print("open_status (0 closed, 1 open):   ");
//  Serial.println(open_status);
//
//  Serial.println("");

  Serial.print("light_level:                      ");
  Serial.println(light_level);
  Serial.print("Lvalue:                           ");
  Serial.println(Lvalue);
  Serial.print("step_Count:                       ");
  Serial.println(stepCount);

  Serial.println("");
  Serial.println("");

  delay(200);
}
