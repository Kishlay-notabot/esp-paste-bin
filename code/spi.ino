#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

ESP8266WebServer server(80);

void handleRoot() {
  File file = SPIFFS.open("/index.html", "r");
  if (file) {
    String html = file.readString();
    file.close();
    server.send(200, "text/html", html);
  } else {
    server.send(404, "text/plain", "File not found");
  }
}

void handleSaveData() {
  if (server.method() == HTTP_POST) {
    StaticJsonDocument<1024> doc;
    deserializeJson(doc, server.arg("plain"));

    File file = SPIFFS.open("/data.json", "w");
    if (file) {
      serializeJson(doc, file);
      file.close();
      server.send(200, "text/plain", "Data saved successfully");
    } else {
      server.send(500, "text/plain", "Failed to open file for writing");
    }
  }
}

void handleGetData() {
  File file = SPIFFS.open("/data.json", "r");
  if (file) {
    StaticJsonDocument<1024> doc;
    deserializeJson(doc, file.readString());
    file.close();
    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);
  } else {
    server.send(404, "text/plain", "File not found");
  }
}

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/save", handleSaveData);
  server.on("/get", handleGetData);
  server.begin();
}

void loop() {
  server.handleClient();
}