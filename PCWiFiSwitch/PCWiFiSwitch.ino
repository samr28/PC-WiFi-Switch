#include <ESP8266WebServer.h>

// #define DEBUG 1

#define HTTP_REST_PORT 80
const int RELAY_PIN = D1;

const char *ssid = "Your WiFi SSID";
const char *password = "Your WiFi Password";
const String deviceName = "A Device";

ESP8266WebServer http_rest_server(HTTP_REST_PORT);

int state = LOW;

String getStateText() {
  return state ? "1" : "0";
}

void getState() {
  http_rest_server.send(200, "text/html", getStateText());
}

void changeState() {
  String post_body = http_rest_server.arg("plain");
  if (post_body == "1") {
    state = 1;
    digitalWrite(RELAY_PIN, HIGH);
  } else if (post_body == "0") {
    state = 0;
    digitalWrite(RELAY_PIN, LOW);
  } else {
    http_rest_server.send(400);
    return;
  }
  http_rest_server.send(200, "text/html", getStateText());
}

void flipState() {
  String post_body = http_rest_server.arg("plain");
  int time = post_body.toInt();
  http_rest_server.send(200, "text/html", "Flipping...");
  digitalWrite(RELAY_PIN, HIGH);
  delay(time);
  digitalWrite(RELAY_PIN, LOW);
}

void getDeviceName() {
  http_rest_server.send(200, "text/html", deviceName);
}

void configureRouter() {
  http_rest_server.on("/name", HTTP_GET, getDeviceName);
  http_rest_server.on("/", HTTP_GET, getState);
  http_rest_server.on("/", HTTP_POST, changeState);
  http_rest_server.on("/flip", HTTP_POST, flipState);
}

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
    delay(10);
  #endif

  // Init relay pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  #ifdef DEBUG
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
  #endif

  WiFi.hostname("PCWiFiSwitch-"+deviceName);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    // Wait a bit and then try again
    delay(500);
    #ifdef DEBUG
        Serial.print(".");
    #endif
  }

  #ifdef DEBUG
    Serial.println("");
    Serial.println("WiFi connected");
  #endif

  // Set routes
  configureRouter();

  // Start the server
  http_rest_server.begin();

  #ifdef DEBUG
    Serial.println("Server started");
    // Print the IP address
    Serial.print("Use this URL : ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
  #endif
}

void loop(){
  http_rest_server.handleClient();
}
