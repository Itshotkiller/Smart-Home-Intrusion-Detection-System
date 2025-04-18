#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "AndroidAP_8532";
const char* password = "12121212";
const char* serverUrl = "http://192.168.139.221:3000/alert";

#define PIR_PIN 4

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (digitalRead(PIR_PIN) == HIGH) {
    sendAlert("motion", "Motion detected in home");
    delay(5000);
  }
  delay(100);
}

void sendAlert(String type, String message) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    
    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "application/json");
    
    String payload = "{\"type\":\"" + type + "\",\"message\":\"" + message + "\"}";
    int httpCode = http.POST(payload);
    
    if (httpCode > 0) {
      Serial.printf("HTTP POST code: %d\n", httpCode);
    } else {
      Serial.println("HTTP POST failed");
    }
    
    http.end();
  }
} 


