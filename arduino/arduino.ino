#include <SoftwareSerial.h>
#include <Wire.h>

/* pH */
#define pHPin A0
#define Offset    31.91785714
#define k         -6.357142857
#define samplingInterval  20
#define printInterval     800
#define ArrayLenth        40    // times of collection
int pHArray[ArrayLenth];   // Menyimpan nilai rata2 hasil pembacaan sensor
int pHArrayIndex = 0;
static double pHValue, voltage;

/* MQ135 Gas Sensor */
#define mqPin A1
float gas= 0;

/* Ultrasonic Sensor */
#define  trig  3
#define  echo  4
long duration;
float distance, ta;
int t = 26.5;                      //distance sensor from ground (cm)

/* DS18B20 Temperature Sensor */
#define wirebus 5
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(wirebus);
DallasTemperature DS18B20(&oneWire);
float wtrTemp;

SoftwareSerial ArduinoUno (7, 6);

void setup() {
  Serial.begin(115200);
  ArduinoUno.begin(9600);

  pinMode(trig, OUTPUT);                
  pinMode(echo, INPUT);                
  pinMode(wirebus, INPUT);
  pinMode(A0, INPUT);

  DS18B20.begin();
}

void loop() {
  ph();
  mq();
  ultrasonic();
  temp();
  delay(2000);
}

void ph() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();

  if (millis() - samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = analogRead(pHPin);
    if (pHArrayIndex == ArrayLenth) pHArrayIndex = 0;

    voltage = avergearray(pHArray, ArrayLenth) * 5.0 / 1024;
    pHValue = k * voltage + Offset;
    samplingTime = millis();
  }

  // Setiap 800 mili detik, print a hasil pembacaan, ubah nyala kondisi LED
  if (millis() - printTime > printInterval) {
    Serial.print("pH value: ");
    Serial.println(pHValue, 2);

    pHValue = pHValue + 2000;
    ArduinoUno.print(pHValue);
    ArduinoUno.println("\n");
  }
}

double avergearray(int* arr, int number) {
  int i;
  int max, min;
  double avg;
  long amount = 0;
  if (number <= 0) {
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }

  if (number < 5) { //less than 5, calculated directly statistics
    for (i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  } else {
    if (arr[0] < arr[1]) {
      min = arr[0]; max = arr[1];
    }
    else {
      min = arr[1]; max = arr[0];
    }
    for (i = 2; i < number; i++) {
      if (arr[i] < min) {
        amount += min;      //arr<min
        min = arr[i];
      } else {
        if (arr[i] > max) {
          amount += max;  //arr>max
          max = arr[i];
        } else {
          amount += arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount / (number - 2);
  }//if
  return avg;
}

void mq()
{
  gas = analogRead(mqPin);
  voltage = analogRead(mqPin) * (5.0 / 1023.0);

  Serial.print("Gas : ");
  Serial.println(gas, 3);

  gas = gas + 4000;
  ArduinoUno.print(gas);
  ArduinoUno.println("\n");
}

void ultrasonic()
{
  digitalWrite(trig, LOW);          // Makes trigPin low
  delayMicroseconds(2);             // 2 micro second delay
  digitalWrite(trig, HIGH);         // tigPin high
  delayMicroseconds(10);            // trigPin high for 10 micro seconds
  digitalWrite(trig, LOW);          // trigPin low

  duration = pulseIn(echo, HIGH);   //Read echo pin, time in microseconds
  distance = duration * 0.034 / 2;  //Calculating actual/real distance
  ta = t - distance;                //Calculating water level

  Serial.print("Distance : ");
  Serial.println(ta, 1);
  
  ta = ta + 6000;
  ArduinoUno.print(ta);
  ArduinoUno.println("\n");
}

void temp()
{
  DS18B20.requestTemperatures(); 
  wtrTemp = DS18B20.getTempCByIndex(0);

  int newTemp = ((wtrTemp + 0.05) * 10); //fix temperature value to 1 decimal place.
  wtrTemp = (newTemp / 10.0);

  Serial.print("Temperature: ");
  Serial.println(wtrTemp, 1);

  wtrTemp = wtrTemp + 8000;
  ArduinoUno.print(wtrTemp);
  ArduinoUno.println("\n");
}
