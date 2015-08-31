/*
 * This sketch aims to read thermal meter upon request.
 * ESP8266 is set as a server. When you visit its IP address in a browser, 
 * a request will be triggered and sent to the thermal meter. As a result,
 * a data package will be returned to PC via ESP8266 and in the end exhibited
 * on the webpage you visit at the beginning.
 * This sketch is free to share and all kinds of suggestions are appreciated.
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

const char *ssid = "...";  // Change to your wifi ssid
const char *password = "...";  // Change to your wifi password
const int BUFFER_SIZE=128;
byte buffer[BUFFER_SIZE];
byte *buffer_pointer;
char request[19]={0xFE,0xFE,0xFE,0x68,0x20,0x87,0x10,0x45,0x31,0x00,0x11,0x11,0x01,0x03,0x1F,0x90,0x00,0x6A,0x16};  // Array of hexadecimal bytes, the only data type that suits the thermal meter

WiFiServer server ( 80 );

void setup ( void ) {
  
  Serial.begin ( 9600 );
  Serial.setTimeout(3000);  // Not necessary, the default setting (i.e. 1000 milliseconds) can do the job
  WiFi.begin ( ssid, password );
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);  // Initialize the led

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) { 
    delay (500);
  }

  server.begin();

  digitalWrite(2, 1);  // Signal, indicates that wifi is successfully connected, you may use serial monitor if you have enough GPIO pins for output
}

void loop () {
  
  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  while(!client.available()){
    delay(1);
  }
  
  digitalWrite(2, 0);  // Signal, indicates a client is connected
  
  Serial.write(request,19);  // Send out the reading request as an array of bytes using stream.write (val,len)

  while ( !Serial.available() ){
    delay (1);
  }

  while ( Serial.available()>0 ){
    Serial.readBytes(buffer,128);
    }

  buffer_pointer=buffer;
  
  digitalWrite(2, 1);  // Signal, indicates reading request has been sent
  delay (500);
  digitalWrite(2, 0);
  delay (500);
  
  client.print ("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nThe msg is ");
  // Here a for loop is applied to output the data package byte by byte on website, really clumsy, but failed using client.write, haven't figured out why
  for (int l=0;l<=74;l++) {
    client.printf ("%02X",*buffer_pointer);
    if ( (l>=69) && (*buffer_pointer==0x16) ) {
      break;
    }
    buffer_pointer++;
  }
  client.print ("</html>\n");
  

  digitalWrite(2, 1);
  delay (500);
  digitalWrite(2, 0);
  delay (500);
  digitalWrite(2, 1);
  delay (500);
  digitalWrite(2, 0);  // Signal, indicates that package has been sent
  
  client.stop();
}
