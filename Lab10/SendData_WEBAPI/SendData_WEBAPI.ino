#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#define DHTTYPE DHT11
#include "DHT.h"
#define DHTPIN D4
DHT dht(DHTPIN, DHTTYPE);

const char *SSID = "FPT ";  // Remove the extra space at the end
const char *PASSWORD = "0985559059";
const char *URL = "http://192.168.1.252:5555/api/sensors";

ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
HTTPClient http;

void setup() {
  Serial.begin(115200);
  
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID, PASSWORD);

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void postJsonData() {
  Serial.print("connecting to ");
  
  if (WiFiMulti.run() == WL_CONNECTED) {
    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, URL)) {  
      Serial.print("[HTTP] POST...\n");
      
      StaticJsonDocument<JSON_OBJECT_SIZE(5)> doc;
      dht.begin();
      float h = dht.readHumidity();
      float t = dht.readTemperature();

      doc["id"] = "nBnEgCDL6lclwlFdtrzv";
      doc["name"] = "thietbi1";
      doc["temperature"] = "24";  // Use the actual temperature value instead of a string
      doc["humid"] = "34";        // Use the actual humidity value instead of a string
      doc["atTime"] = "2021-07-06";

      char output[2048];
      serializeJson(doc, output);

      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(output);
      Serial.println(httpCode);
      
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        Serial.println(payload);
        
        DynamicJsonDocument docResponse(2048);
        DeserializationError err = deserializeJson(docResponse, payload);
        if (err) {
          Serial.print(F("deserializeJson() failed with code "));
          Serial.println(err.c_str());
        } else {
          const char *name = docResponse["name"]; // No need to use .as<char*>()
          Serial.println(name);
        }
      } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
      Serial.println("closing connection");
    }
  }
}

void loop() {
  postJsonData();
  delay(10000);
}
