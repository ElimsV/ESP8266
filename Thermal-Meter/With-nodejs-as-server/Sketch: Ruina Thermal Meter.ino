#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

const char *ssid = "***";  // Your wifi ssid
const char *password = "***";  // Your wifi password
const char *host = "***";  // Your server IP address
const int port = ***;  // Your server port

const int BUFFER_SIZE=128;
byte buffer[BUFFER_SIZE];
int msgSize;

WiFiClient client;

void setup() {
  Serial.begin ( 9600 );
  
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);  // LED lights off

  WiFi.begin ( ssid, password );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) { 
    delay (500);
  }

  while ( !client.connect(host, port) ) {
    delay (10);
  }
  digitalWrite(2, 1);   // LED lights on after esp connected to WiFi
  client.setTimeout (10000);

}

void loop() {

  while ( !client.connected() ) {
    client.connect(host, port);
    delay (500);
    digitalWrite(2, 0);  // LED lights off when esp hasn't been connected to the io server
  }

  digitalWrite(2, 1);  // LED lights on when esp is connected to server

  while ( !client.available() ) {
    delay (10);
  }
  
  client.readBytes ( buffer,128 );  
  msgSize = sizeof ( buffer );
     
  if ( 16 < msgSize < 21 ) {  // Request message coming from server should be sent to the meter
    Serial.write ( buffer,19 );
    while ( !Serial.available() ) {  // Wait for the data package from the meter
      delay (10);
    } else {
      break;
    }
    Serial.readBytes ( buffer,128 );
    client.write ( (const char*)buffer,72 );
  } 
}
