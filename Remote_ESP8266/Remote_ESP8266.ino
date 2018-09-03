#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <PubSubClient.h>

#define IR_LED 4  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.



const char* ssid = "Bah";
const char* password = "5133738457";
const char* mqtt_server = "18.216.146.0";


ESP8266WebServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);

const int led = 13;



void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleTVON() {
  irsend.sendNEC(0x4CB0FADC, 32); // turn on 
  server.send(200, "text/json", "TV turned ON or OFF");
  digitalWrite(led, 0);
}

void handleUP() {
  irsend.sendNEC(0x2FD7887, 32); // turn on 
  server.send(200, "text/json", "Volume Down");
  digitalWrite(led, 0);
}

void handleDOWN() {
  irsend.sendNEC(0x2FD58A7, 32); // turn on 
  server.send(200, "text/json", "Volum UP");
  digitalWrite(led, 0);
}



void handleNotFound() {
  server.send(404, "text/plain", "error doesn't exist");
}
 
void setup(void) {
  irsend.begin();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  client.setServer(mqtt_server, 1883);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/TV", handleTVON);
  server.on("/UP", handleUP);
  server.on("/DOWN", handleDOWN);

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop(void) {
  if (!client.connected()) {
    reconnect();
  }
 
  server.handleClient();
}
