##Introduction
This project aims to build a simplified AMI (Advanced Metering Infrastructure) system using esp8266, Ruina heat meter and nodejs i/o server.

In this project, the esp8266 purely functions as a transmitting unit in the smart metering system. To be more detailed, the
esp8266 receives metering requests from the nodejs i/o server via wifi and sends them to the heat meter. Similarly, when the heat
meter sends back data packages, the esp8266 pass them on to a certain port of the i/o server. In this manner, esp8266 won't process the data passing
it but merely does the delivery work.

##Crucial points
1. Once again I use a led as an indicator of the operational status (i.e. the led will turn or off when a certain task has been done);

2. The data transmitted between server and the heat meter are always sent as series of bytes, hence function *stream*.write and *stream*.readBytes must be applied.

3. To make client.write function well, you should make format declaration as following:
   ```c++
   client.write ( (const char*)buffer,len );
   ```
   
4. The client only need to be declared and connected to the server once, while in the loop part you might re-connected it to the server according to connection status.

##What can be done in future
As this is a very simple model of AMI, to be more practical, an eletronic valve could be used. When the server finds the customer
has consumed too much heat without paying enough money, it will send a "switch off" command to esp8266 which will in return close the valve.
