#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <math.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>


int TempC;
int TempF;

const char* ssid = "Bah";
const char* password = "5133738457";
const char* mqtt_server = "18.216.146.0";


ESP8266WebServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);

const int led = 13;



double Thermistor(int RawADC) {
 double Temp;
 Temp =log(10000.0/(1024.0/RawADC-1)); // for pull-up configuration
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;            // Convert Kelvin to Celcius
 return Temp;
}


void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleTempC() {
  TempC = int(Thermistor(analogRead(A0)));
  TempF = int(TempC*1.8 + 32);
  String TEMPC = "{TempC:" + String(TempC)+",TempF:"+String(TempF)+"}";
  Serial.println(TEMPC);
  digitalWrite(led, 1);
  server.send(200, "text/json", TEMPC);
  digitalWrite(led, 0);
}



void handleNotFound() {
  server.send(404, "text/plain", "error doesn't exist");
}
 
void setup(void) {
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
  server.on("/TempC", handleTempC);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

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
  int TC = int(Thermistor(analogRead(A0)));
  int TF = int(TC*1.8 + 32);
  String Ctemp = String(TC);
  String Ftemp = String(TF);
//  Serial.println(Ctemp);
  client.publish("TEMPC", Ctemp.c_str());
  client.publish("TEMPF", Ftemp.c_str());
}
