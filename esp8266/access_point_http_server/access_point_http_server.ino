#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

// create a webserver running on port 80
ESP8266WebServer server(80);

// define pin settings
const int LED_PIN = 16;

// define access point settings
const char * ssid = "ESP8266 Access Point";
const char * password = "esp8266arduino";

// define function prototypes for HTTP handlers
void handleRoot();
void handleLED();
void handleNotFound();

void setup() 
{
    // initialize the debugging serial port
    Serial.begin(115200);
    delay(10);
    Serial.println('\n');

    // setup LED pin
    pinMode(LED_PIN, OUTPUT);

    // start the access point
    WiFi.softAP(ssid, password);
    Serial.print("Access Point \"");
    Serial.print(ssid);
    Serial.println("\" started");

    // print the IP address
    Serial.print("IP address:\t");
    Serial.println(WiFi.softAPIP());

    // start handlers for HTTP requests
    server.on("/", handleRoot);
    server.on("/LED", HTTP_POST, handleLED);
    server.onNotFound(handleNotFound);

    // start the server
    server.begin();
    Serial.println("HTTP server started");
}

// program main
void loop(void) 
{
    // listen for HTTP requests from clients
    server.handleClient();
}

// handler for requests at root path
void handleRoot() 
{
    // send response message with 200 HTTP status code
    server.send(200, "text/plain", "Hello world!");
}

// handler for requests at LED path
void handleLED() 
{                          
    // change LED state
  digitalWrite(LED_PIN,!digitalRead(LED_PIN));

  // send the response
  server.send(200, "text/plain", "LED state changed!");
}

// handler for undefined paths
void handleNotFound() 
{
    // send response message with 404 HTTP status code
    server.send(404, "text/plain", "404: Not found");
}
