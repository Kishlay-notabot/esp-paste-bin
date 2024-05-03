#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Wi-Fi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Web server object
ESP8266WebServer server(80);

// Function to generate the HTML page
String generateHTMLPage() {
  String html = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<title>ESP8266 Web Server</title>"
                "</head>"
                "<body>"
                "<h1>Welcome to the ESP8266 Web Server</h1>"
                "<p>This is a basic HTML page served by the ESP8266.</p>"
                "</body>"
                "</html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html", generateHTMLPage());
}

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to Wi-Fi");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up the web server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}