#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

const char* host = "ecse-three-led-api.onrender.com";


const int ledPin1 = 12;
const int ledPin2 = 13;
const int ledPin3 = 14;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  Serial.println("");

  Serial.println("Loading");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    String http_response;


    String url = "https://" + String(host) + "/api/state";
    http.begin(url);
    http.addHeader("X-API-Key", API_KEY);

    int httpResponseCode = http.GET();

    Serial.println("");
    Serial.println("");

    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        Serial.print("Response from server: ");
        http_response = http.getString();
        Serial.println(http_response);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();

      StaticJsonDocument<1024> doc;

      DeserializationError error = deserializeJson(doc, http_response);

      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }
      
      bool light_switch_1 = doc["light_switch_1"]; 
      bool light_switch_2 = doc["light_switch_2"]; 
      bool light_switch_3 = doc["light_switch_3"];
  
      Serial.println("");
      Serial.println(light_switch_1);
      Serial.println(light_switch_2);
      Serial.println(light_switch_3);
      digitalWrite(ledPin1, light_switch_1);
      digitalWrite(ledPin2,light_switch_2);
      digitalWrite(ledPin3, light_switch_3);
      Serial.println("LED states updated");
      
  }
  else {
    return;
  }
}