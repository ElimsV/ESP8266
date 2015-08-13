#Introduction
This sketch is basically based on codes from http://www.arduinesp.com/thingspeak, many thanks to their sharing.

This is a simple exemple of using esp8266 as a client to send sensor data to ThingSpeak.com so that data could be shared 
by all of the world. Wiring is the same as what we do in the WebServer case. You just need to alter apiKey, ssid, and password
to give it a try!

#What could be done in future
1. Customize the refreshing time interval, which will be of course correlated with the internal settings of ThingSpeak.com, to
   keep the diagram neat and clean.
2. Add more sensors and show all the data points in one or multiple diagraphs.
3. Customize the axis lables, line styles, and something like that.
4. Substitute the original x values, which is defaultly set as time line, with other information to suit your need.

