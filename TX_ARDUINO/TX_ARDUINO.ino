#include "DHT.h"

#define Buzzer 10
#define RELAY1 11 // untuk penyiraman
#define RELAY2 12 // belum terdefinisi

#define pinDHT 13
#define DHTTYPE DHT11
DHT dht(pinDHT, DHTTYPE);

const int SENSOR_HUMIDITY = A0;
const int SENSOR_PH_SOIL = A1;

String data[4];

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  data[3] = "OFF";
}

void loop() {
  handleReadHumiditySensor();
  handleReadTemperatureSensor();
  handleReadPHSensor();
  String RXRequest = "";
  while(Serial.available() > 0){
    RXRequest += char(Serial.read());
  }
  RXRequest.trim();
  if(RXRequest == "GET"){
    String sendingData = data[0] + ":" + data[1] + ":" + data[2] + ":" + data[3];
    Serial.println(sendingData);
  }
  
//  Serial.println("===============DATA SENSOR===============");
//  Serial.println("HUMIDITY :" + data[0]);
//  Serial.println("TEMPERATURE :" + data[1]);
//  Serial.println("PH SOIL :" + data[2]);
//  Serial.println("WATERING :" + data[3]);
//  Serial.println("=========================================");
  RXRequest = "";
  delay(1000);
}

void handleReadHumiditySensor(){
  int humidity = analogRead(SENSOR_HUMIDITY);
  data[0] = humidity;
  if(humidity > 900){
    digitalWrite(RELAY1, LOW);
    data[3] = "ON";
    if(humidity < 500){
      digitalWrite(RELAY1, HIGH);
      data[3] = "OFF";
    }else{
      playBuzzer();
    }
  }else{
    if(humidity < 500){
      digitalWrite(RELAY1, HIGH);
      data[3] = "OFF";
    }
  }
  
}

void handleReadTemperatureSensor(){
  float temperature = dht.readTemperature();
  if(!isnan(temperature)){
    data[1] = temperature;
  }
}

void handleReadPHSensor(){
  data[2] = analogRead(SENSOR_PH_SOIL);
}

void playBuzzer(){
  digitalWrite(Buzzer, HIGH);
  delay(100);
  digitalWrite(Buzzer, LOW);
  delay(200);
}
