int i=0;
volatile unsigned int k=0;

const int Trigger = 6;   //Pin digital 6 para el Trigger del sensor
const int Echo = 7;      //Pin digital 7 para el Echo del sensor
int ref;                 //valor al que lo quiero mandar en cm
int IN1 = 3;             //Input sentido de giro motor
int IN2 = 2;             //Input sentido de giro motor
int vel = 5;             //Pin digital 5 (permite pwm)
float error;             //Diferencia entre referencia y distancia sensada
float errorp= 0;         //Error Pasado
float errorpp = 0;       //Error Pasado Pasado
int pwm;                 //PWM que acciona sobre el sistema
int base;                //Complementario a la accion de control para generar el valor de PWM
int U ;                  //Accion de control actual
int Up = 0;              //Accion de control Pasada
int Upp = 0;             //Accion de control Pasada Pasada

#define arraySize 10                  //Variables para la lectura de la informacion que pongo en el monitor en serie
char inData[arraySize];
short index = 0;
char inChar = 0;
int valor = 0;           

  
void setup() 
{
  Serial.begin(9600);                 //iniciailzamos la comunicación
//PASO 1 - Deshabilita interrupciones
SREG=0b01111111;
//PASO 2 - Habilita interrupciones por desbordamiento
TIMSK2=0b00000001; 
//PASO 3 - Borra registros que guardan las cuentas del timer
TCNT2=0;
//PASO 4 - Configura preescala del timer
TCCR2B=0b00000101;
//PASO 5 - Habilita nuevamente las interrupciones
SREG=0b10000000;

  
  pinMode(Trigger, OUTPUT); 
  pinMode(Echo, INPUT);
  digitalWrite(Trigger, LOW);  //Iniciamos el pin con 0
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(vel, OUTPUT);
}

void loop()
{

 
 if (Serial.available()>0)
 {
    while(Serial.available() > 0 && index < arraySize-1)
    {
        {
        inChar = Serial.read();
        inData[index] = inChar;            //Determinacion de informacion introducida al sistema por medio del monitor en serie
        index++;
        inData[index] = '\0';              // Add a null at the end
        }
    }
  }
  
  valor=atoi( inData );
  index = 0;
  ref=valor;

  float t; //tiempo que demora en llegar el echo
  float d; //distancia en centimetros


if (ref>31)  //Limito el sistema por cuestiones físicas(de 6 a 31)
{           //Por lo tanto Error max = 25
  ref =31;
}
if (ref<6)
{
  ref=6;
}


//DEVUELVE DISTANCIA Y ERROR
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);               //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  t = pulseIn(Echo, HIGH);             //obtenemos el ancho del pulso
  d = t/59;                            //escalamos el tiempo a una distancia en cm
  error=abs(d-ref);
 
  
 //LE DOY EL SENTIDO SEGUN VALOR DIFERENCIA
    if ((d-ref)>0)
    {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(vel, pwm);
    }
    if((d-ref)<0) 
    {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(vel, pwm);
    }
    if(error<0.7)//PONGO EN LOW LOS PINES DEL MOTOR
    {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);  
    }
  
//IMPRIMO DATOS EN EL MONITOR SERIE
  Serial.print("Distancia: ");
  Serial.print(d);      
  Serial.print(" cm");
  Serial.print("      Error: ");
  Serial.print(error);      
  Serial.print(" cm");
  Serial.print("      Vel: ");
  
  Serial.print(pwm);      
  Serial.print(" pwm");
  
  Serial.print("    Posición deseada: ");
  Serial.print(ref);
  Serial.println(" cm");
        
}

ISR(TIMER2_OVF_vect) // Usamos este timer de 8 bits para realizar las interrupciones con un prescaler de 128 que es el que mejor se adapta a nuestra planta. La eleccion de manera mas
{                    // precisa de la eleccion del timer se encuentra en el informe adjunto.

      base = 120;                                                                  // Usamos una base de 120 de PWM , usamos 120 porque experimentalmente fuimos probando con el menor 
      U = 1.995*Up + 0.995*Upp + 4.803*error - 0.9576*errorp + 0.4773*errorpp;     // PWM posible para poder mover el carro, asi el efecto de control podia tener mayor influencia sobre
      U = map(U,0,450,0,135);                                                      // el sistema. Esto significa que si yo tomara 200 por ejemplo de base, el mapeo de los 450 valores 
      Upp = Up;                                                                    // seria reducido a 50 valores , esto significa que 9 valores del intervalo en este mapeo simbolizan
      Up = U;                                                                      // 1 valor en la accion de control de PWM. En cambio, con la base en 120 hay 3.333 valores en este mapeo
      errorpp = errorp;                                                            // simbolizan 1 valor en la accion de control de PWM. Es un aumento de casi un 300% en la sensibilidad de
      errorp = error;                                                              // de la accion de control. El valor 450 lo deducimos por experimentalmente ploteando los valores maximos
      pwm = base + U;                                                              // de U para el error mas grande , esto seria pedirle al carro que vaya de un extremo al otro.
       
}
