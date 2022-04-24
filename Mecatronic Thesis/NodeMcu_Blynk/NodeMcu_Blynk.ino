                                                                                 // Defino la conexion de todos los pines de la huerta
#define trigPin D7
#define echoPin D8
long duration;
float distance;
float percentage;

#define DHTPIN D2          

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "JIpPXhBWD03tlWEKfY5ulEBUytAnmypt";                               // Codigo de autenticacion para linkear con la app de Blynk

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Fibertel WiFi493 2.4GHz";                                        // Nombre de la red de WiFi
char pass[] = "nicolas1234";                                                    // Contraseña de WiFi

//#define DHTTYPE DHT11     // DHT 11
#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321                               // Modelo de nuestro modulo Humedad y Temperatura Ambiente
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

void sendSensor()

{
  float h = dht.readHumidity();                                                 // Lectura de Humedad
  float t = dht.readTemperature();                                              // Lectura de Temperatura

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
                                                                                // El proceso de lectura de distancia con el sensor HC-SR04
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
                                                                               // Se envian los datos de temperatura, humedad y distancia a la app de Blynk

  if (distance < 16.00 && distance > 3.00) 
  {
  percentage = map(distance,16.00, 3.00, 0.00, 100.00);          
    }

  if (distance > 16.00) 
  {
  percentage = 0.00;          
    }

  if (distance < 3.00) 
  {
  percentage = 100.0;          
    }
  
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V7, h);
  Blynk.virtualWrite(V9, percentage);
  
  Serial.print("La temperatura es: ");
  Serial.print(t);
  Serial.print(" ºC || ");
  
  Serial.print("La humedad relativa es: ");
  Serial.print(h);
  Serial.print(" % || ");
  
  Serial.print("El agua esta a una distancia de ");
  Serial.print(distance);
  Serial.println(" || ");

  Serial.print("Porcentaje de agua resatante en la reserva es de ");
  Serial.print(percentage);
  Serial.println(" || ");


  Serial.println("");
}

void setup()
{
                                                                                 // Defino si los pines son INPUT (vamos a usar ese pin para un sensor) 
                                                                                 // o OUTPUT (vamos a usar ese pin para un actuador)
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input

  Blynk.begin(auth, ssid, pass);
  
  dht.begin();
  timer.setInterval(1000L, sendSensor);

  Serial.begin(9600);
}

void loop()
{
  Blynk.run();
  timer.run();
}
