#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "MikroTik-232859";
const char* password = "Tvp123456#";
const char* lightSensorEndPoint = "http://192.168.88.221:3000/light-sensor";

const int pinoLed = D0; //PINO DIGITAL UTILIZADO PELO LED
const int pinoLDR = A0; //PINO ANALÓGICO UTILIZADO PELO LDR
const int pinoRele = D2;

void setup(){  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("Connected");
  Serial.println(WiFi.localIP());
  Serial.print(ssid);
  
  pinMode(pinoLed, OUTPUT);
  pinMode(pinoLDR, INPUT);
  pinMode(pinoRele, OUTPUT);
}   
 
void loop(){
 delay(250);
 int LDRData = analogRead(pinoLDR);
 WiFiClient client;
 HTTPClient http;
 String parameter;

 http.begin(client, lightSensorEndPoint);
 http.addHeader("content-type", "application/json");

 StaticJsonBuffer<500> jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
 
 root["ldr_value"] = LDRData;
 root.printTo(parameter);
 
 int httpResponseCode = http.POST(parameter);
 String payload;

 if(httpResponseCode > 0){
  payload = http.getString();
  JsonObject& response = jsonBuffer.parseObject(payload);
  int led_state = response["led_state"];
  
  Serial.println(" ");
  Serial.print(httpResponseCode);
  Serial.print(" ");
  Serial.println(payload); 

  if(led_state == 1){
    digitalWrite(pinoLed, HIGH);
    digitalWrite(pinoRele, HIGH);
  }else if(led_state == 0){
    digitalWrite(pinoLed, LOW);
    digitalWrite(pinoRele, LOW);
  }

  }else{
    digitalWrite(pinoLed, HIGH);
    delay(50);
    digitalWrite(pinoLed, LOW);
    delay(50);  
   }
   
  http.end();
}
