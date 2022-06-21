#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SoftwareSerial.h>

#define FIREBASE_HOST "smart-farming-3197a-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "XEx97RQxx0KUXuRDzS37ORChxUPfGyflH0cOopAY"
#define WIFI_SSID "iPhone"
#define WIFI_PASSWORD "password"

SoftwareSerial DataSerial(12, 13); // RX,TX

const int GREEN_LED  = 5; //GPIO5
const int YELLOW_LED = 4; //GPIO4

String results[4];
unsigned long previousMillis = 0;
const long interval = 3000;

void setup() {
  Serial.begin(9600);
  DataSerial.begin(9600);
  
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  handleCheckWifiStatus();
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;

    String data = "";
    while(DataSerial.available() > 0){
      data += char(DataSerial.read());
    }
    data.trim();
    if(data != ""){
      int index = 0;
      for(int i=0; i < data.length(); i++){
        char delimiter = ':';
        if(data[i] != delimiter)
          results[index] += data[i];
        else
          index++;
      }

      if(index == 3){
        Serial.println("=========================================");
        Serial.println("KELEMBABAN : " + results[0]);
        Serial.println("SUHU UDARA : " + results[1]);
        Serial.println("PH TANAH   : " + results[2]);
        Serial.println("PENYIRAMAN : " + results[3]);
        Serial.println("=========================================");

        
         Firebase.setString("/humidity", results[0]);
         Firebase.setString("/temperature", results[1]);
         Firebase.setString("/ph", results[2]);
         Firebase.setString("/watering", results[3]);
         if (Firebase.failed()) {
          Serial.println(Firebase.error());
         }
         
      }
      
      results[0] = "";
      results[1] = "";
      results[2] = "";
      results[3] = "";
    }
    DataSerial.println("GET");
  }
}

void handleCheckWifiStatus(){
  if(WiFi.status() == WL_CONNECTED){
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
  }else{
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
  }
}
