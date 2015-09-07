##Introduction
This sketch aims to collect thermal data from an Ruina thermal meter using esp8266 as a server.
When you visit esp8266's ip address in a browser, your PC is connected to the server as a client, and thus a request will be triggered
and sent to the thermal meter. Immediately the meter will response to that and send a data package back, which is capsulated 
according to Ruina Company's protocal. Transmitted by the esp8266, the package is at last exhibited on the webpage you visit at the very 
beginning.
By the way, the corresponding decapsulation process is demonstrated in another folder named "decapsulation" in this repository. 

##Crucial points
Leave out the some fundamental knowledge, you should note that: 
  1. UTXD and URXD should be correctly connected to the chip on PCB board.
  
  2. Since only 1 GPIO pin (i.e. GPIO2) is available after the TX & RX pin has been used (I only got esp8266 esp-01 chip), I used a led as an indicator of the system status (turned on or off when a certain task has been done). I strongly suggest you to buy a wifi chip with more than 4 GPIO pins which will make your life a lot more easier.
  
  3. According to Ruina's communication protocal, the request and the data package are all sent byte by byte as series of hexadecimal digits. Hence serial.write is applied here to send the request to the meter rather than serial.print, and the request message was defined as an array of hexadecimal bytes.
  
  4. Similarly, readBytes was used instead of readString to get all the response in the form of bytes from the thermal meter.
  
  5. Buffer itself is a constant pointer which means the address it pointed to can't be modified so when you want to use for loop and printf function to get the content it pointed to you should define another variable pointer.

##What could be done in future
I am trying to figure out how to use client.write instead of for loop and printf function to send package back to the webpage.
So far I have never passed the compilation once which totally doesn't make sense. I think there is something wrong with WiFiClient.h file.

**UPDATE**
Previously I can't use client.write function has nothing to do with the library file but totally because lacking the format declaration which looks like the following:
```c++
client.write ( (const char*)buffer,len );
```
When you add a symple declaration (const char*) , ta-da, problem solved!
