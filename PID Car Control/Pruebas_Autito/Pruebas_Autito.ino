//para el motor dere.-cho
int MD2=7;//El pin 7 de arduino se conecta al pin L_EN del puente H derecho
int MD1=8;//El pin 8 de arduino se conecta al pin R_EN del puente H derecho
int PWMD=9;//El pin 9 de arduino se conecta con el pin RPWM del puente H derecho
int ED=2;//La señal del encoder derecho ingresa por el pin 2 de arduino (capaz de disparar interrupciones)

//para el motor izquierdo
int MI1=4;//El pin 4 de arduino se conecta al pin L_EN del puente H izquierdo
int MI2=5;//El pin 5 de arduino se conecta al pin R_EN del puente H izquierdo
int PWMI=6;//El pin 6 de arduino se conecta con el pin RPWM del puente H izquierdo
int EI=3;//La señal del encoder izquierdo ingresa por el pin 3 de arduino (capaz de disparar interrupciones)

volatile float  TiempoTotal = 0L;
volatile float  TiempoTotal2 = 0L;

volatile float TiempoRef1;
volatile float TiempoRef2;


volatile float delta;
volatile int a;
volatile float delta2;
volatile int a2;

volatile int n=0;//Contadores
volatile float tc=0;//tiempo de clock, lo cambiamos despues con el prescaler

//PID MD
float errorD;             //Diferencia entre referencia y distancia sensada
float errorpD= 0;         //Error Pasado
float errorppD = 0;       //Error Pasado Pasado
int pwmD = 170 ;                 //PWM que acciona sobre el sistema
int base;                //Complementario a la accion de control para generar el valor de PWM
float UD ;                  //Accion de control actual
int UpD = 0;              //Accion de control Pasada
int UppD = 0;             //Accion de control Pasada Pasada

//PID MI
float errorI;             //Diferencia entre referencia y distancia sensada
float errorpI= 0;         //Error Pasado
float errorppI = 0;       //Error Pasado Pasado
int pwmI = 170;                 //PWM que acciona sobre el sistema
float UI ;                  //Accion de control actual
int UpI = 0;              //Accion de control Pasada
int UppI = 0;             //Accion de control Pasada Pasada

float VtD;
float VtI;

volatile int RanuraD;
volatile int RanuraI;

int TargetD = 200;
int TargetI = 200;

void setup() {
  pinMode(MI1,  OUTPUT); 
  pinMode(MI2,  OUTPUT); 
  pinMode(PWMI, OUTPUT);
  pinMode(MD1,  OUTPUT); 
  pinMode(MD2,  OUTPUT); 
  pinMode(PWMD, OUTPUT);

  pinMode(ED, INPUT); 
  pinMode(EI, INPUT);

  digitalWrite(MD1,LOW);
  digitalWrite(MD2,HIGH);
  //digitalWrite(MD2,LOW);

  
  digitalWrite(MI1,LOW);
  digitalWrite(MI2,HIGH);
  //digitalWrite(MI2,LOW);

  analogWrite(PWMD,pwmD);
  analogWrite(PWMI,pwmI);

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
tc=0.000016; //Tiempo entre dos valores del registro TCNT2
  
Serial.begin(9600);
}

void loop() {

attachInterrupt(digitalPinToInterrupt(ED),VelD,RISING); //Vt MD
attachInterrupt(digitalPinToInterrupt(EI),VelI,RISING); //Vt MI

Serial.print("pwmD: ");
Serial.print(pwmD);
Serial.print(" ");
Serial.print("TargetD: ");
Serial.print(TargetD);
Serial.print("VtD: ");
Serial.print(VtD);
Serial.print(" cm/s   ");
//Serial.print(",");
Serial.print("pwmI: ");
Serial.print(pwmI);
Serial.print(" ");
Serial.print("TargetD: ");
Serial.print(TargetI);
Serial.print("VtI: ");
Serial.print(VtI);
Serial.println(" cm/s");

//Serial.println(pwmD);
//Serial.println(pwmI);


if (VtI > VtD){
  TargetD = TargetD + 1; 
  TargetI = TargetI - 1;
  }
  
if (VtD > VtI) {
  TargetI = TargetI + 1; 
  TargetD = TargetD - 1;
  }

if (pwmD > 250){
TargetD = 0;
}
if (TargetD > 250){
TargetD = 250;
}
if (TargetD < 150){
TargetD = 150;
}

if (pwmI > 250){
TargetI = 0;
}
if (TargetI > 250){
TargetI = 250;
} 
if (TargetI < 150){
TargetI = 150;
}

analogWrite(PWMD,pwmD);
}


ISR(TIMER2_OVF_vect) // Usamos este timer de 8 bits para realizar las interrupciones con un prescaler de 128 que es el que mejor se adapta a nuestra planta. La eleccion de manera mas
{                    // precisa de la eleccion del timer se encuentra en el informe adjunto.
n = n + 1;

base = 120;                                                                   

      errorD = TargetD - pwmD;
      UD = 1.996*UpD - 0.9957*UppD + 0.02002*errorD - 0.03991*errorpD + 0.0199*errorppD;     
      
      if( UD > 250){
        UD = 250;
      }

      UD = map(UD,0,270,0,255);                                                       
      UppD = UpD;                                                                    
      UpD = UD;                                                                    
      errorppD = errorpD;                                                            
      errorpD = errorD;                                                              
      pwmD = UD;                                                              
 



errorI = TargetI - pwmI;
                                                               
      UI = 1.996*UpI - 0.9957*UppI + 0.01501*errorI - 0.02994*errorpI + 0.01493*errorppI;     
      UI = map(UI,0,270,0,255);  
      if( UI > 250){
        UI = 250;
      }                                                     
      UppI = UpI;                                                                    
      UpI = UI;                                                                    
      errorppI = errorpI;                                                            
      errorpI = errorI;                                                              
      pwmI = UI;

analogWrite(PWMI,pwmI);


}

void VelD()
{
  RanuraD = RanuraD + 1;
  
  a=TCNT2;
  TiempoTotal=n*(tc*255)+a*tc;//Usamos la base de tiempo para conocer el tiempo absoluto desde que arranco el programa
  
  if (RanuraD == 1) {
    TiempoRef2 = TiempoTotal;
    }
  if (RanuraD == 11) {  //Se realizo 10/20 de vuelta
     delta = TiempoTotal - TiempoRef2;
     VtD = (3.14*0.024/delta);    //Formula de una circunferencia 2*pi*radio o pi*diametro o en este caso como es media circunferencia lo que buscamos medir pi*radio

    RanuraD = 0;
    }

}

void VelI()
{
  RanuraI = RanuraI + 1;
  
  a2=TCNT2;
  TiempoTotal2=n*(tc*255)+a2*tc;//Usamos la base de tiempo para conocer el tiempo absoluto desde que arranco el programa
  
  if (RanuraI == 1) {
    TiempoRef2 = TiempoTotal2;
    }
  if (RanuraI == 11) {  //Se realizo 10/20 de vuelta
     delta2 = TiempoTotal2 - TiempoRef2;
     VtI = (3.14*0.024/delta);    //Formula de una circunferencia 2*pi*radio o pi*diametro o en este caso como es media circunferencia lo que buscamos medir pi*radio

    RanuraI = 0;
    }

}
