// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>

// Network Credentials
const char* ssid = "AJRB";
const char* password = "password77";

// Create AsyncWebServer Object on Port 80
AsyncWebServer server(80);

// Create Fake Reading for Testing
String testReading(){
  long p = random(0,5);
  Serial.println(String(p));
  return String(p);
}

// Setup and Run Server
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/c1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", testReading().c_str());
  });
  server.on("/c2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", testReading().c_str());
  });
  server.on("/c3", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", testReading().c_str());
  });
  server.on("/c4", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", testReading().c_str());
  });


  // Start server
  server.begin();
}

// Async Server, Nothing Needed in Loop
void loop(){
  
}
