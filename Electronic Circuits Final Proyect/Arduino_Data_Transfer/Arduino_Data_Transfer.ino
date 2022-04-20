/*https://forum.arduino.cc/index.php?topic=492772.0

La idea es que le llegue un valor (180 en este caso), y con ese dato clavo un if que hace que el LED_BUILTIN se prenda y apague por 10s */

//Sender Code

void setup() {
  Serial.begin(9600);
}

void loop() { 
int value=180; //this would be much more exciting if it was a sensor value

  Serial.write(value / 256);
      
  Serial.write(value % 256);
   delay(50); //allows all serial sent to be received together
}

//Receiver Code


void setup() {
  Serial.begin(9600);
pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  int val;

      
      while (!Serial.available()){}
//delay(100); //allows all serial sent to be received together
    byte b1 = Serial.read();
    while (!Serial.available()){}
  //  delay(100); //allows all serial sent to be received together
    byte b2 = Serial.read();

    val = b1  + b2 *256 ; 
    Serial.println(val);
  
  if (val>0) {digitalWrite(LED_BUILTIN, HIGH);
  delay(10000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(10000); }
}
