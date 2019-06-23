#include <ESP8266WiFi.h>

// #define DEBUG 1

const char *ssid = "";
const char *password = "";

const int relayPin = D1;
WiFiServer server(80);

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
  delay(10);
#endif

  // Init relay pin
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

#ifdef DEBUG
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
#endif

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
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

  // Start the server
  server.begin();

#ifdef DEBUG
  Serial.println("Server started");
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
#endif
}

void loop()
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

// Wait until the client sends some data
#ifdef DEBUG
  Serial.println("new client");
#endif

  while (!client.available())
  {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
#ifdef DEBUG
  Serial.println(request);
#endif
  client.flush();

  //Match the request, checking to see what the currect state is
  int value = LOW;
  if (request.indexOf("/relay=ON") != -1)
  {
    digitalWrite(relayPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/relay=OFF") != -1)
  {
    digitalWrite(relayPin, LOW);
    value = LOW;
  }

  buildHTML(client, value);

  delay(1);
#ifdef DEBUG
  Serial.println("Client disconnected");
  Serial.println("");
#endif
}

String buildHTML(WiFiClient client, int value)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");

  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  // Head
  client.println("<head>");
  // Bootstrap
  client.println("<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css' integrity='sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T' crossorigin='anonymous' />");
  // MD Bootstrap
  client.println("<link href='https://mdbootstrap.com/wp-content/themes/mdbootstrap4/css/compiled-4.5.9.min.css?ver=4.5.9]' rel='stylesheet' />");
  client.println("</head>");
  
  // Body
  client.println("<body>");
  client.println("<div class='container mt-3'><div class='row'><div class='col'><h1>PC Remote Start</h1></div></div>");
  client.println("<div class='row'><div class='col'><div class='card mt-3'><div class='header pt-3 pb-3 blue-gradient'><div class='row d-flex justify-content-center'><h3 class='white-text pt-3 font-weight-bold'>SYSTEM NAME</h3></div></div><div class='card-body'><div class='row'><div class='col'>");
  client.print("<h5>Switch is currently: ");
  if (value == HIGH) {
    client.print("ON");
  } else {
    client.print("OFF");
  }
  client.print("</h5></div><div class='col'><a href=\"/relay=ON\"><button class='btn btn-success pull-right'>Turn on</button></a><a href=\"/relay=OFF\"><button class='btn btn-danger pull-right'>Turn off</button></a></div></div></div></div></div></div></div>");
  client.println("</body>");
  client.println("</html>");
}
