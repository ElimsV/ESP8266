##Introduction
This is a simple while practical example to illustrate how to decapsulate a reletive long package. All the codes are writen
regards to Rui Na Thermal Meter. Although ideas are similar, you ought to modify some specific details when apply this sketch
in other cases.

Basically this is all about for loops, chars, char*s, and many assignment statements. As for me, the trick part is the proper use
of chars and char*s. char takes up one byte of memory that stores one character value, but it seems to me that it is also capable
of assigning a character string into one variable. char* is very special. It declares particularly a string ended with '\0'. Besides
it can declare an array of strings, like:
```
char unit1[3]="02";
char unit2[3]="05";
char unit3[3]="08";
char unit4[3]="0A";

char* unitTotal[16]={
  unit1,unit2,unit3,unit4,
};
```
In the above example, I use
```
char unit1[3]="02";
```
instead of
```
char unit1[2]="02";
```
This is because once you declare a string or a character, it is highly recommanded to add one on the size of your string on the 
basis of its original length and to be more precise you can even manually include '\0' in your string. '\0' symbol is used to 
declare the end of a string. Usually system will automatically assign it at the end of a string, however the premise is you have 
left one more byte for it to do so.

Another thing that bothered me was the serial buffer size. First due to practical constraints at that time, I planed to use an 
arduino pro mini to test my code, however I found it can't read in the whole package. After ruling other other possibilities 
I found the the default serial buffer size of mini is only 64 bytes which is apparently too small for my need. As a result I 
tested directly on esp8266 instead. The RX buffer on the esp is something like 256 bytes. Of course you can just change the buffer
size of arduino manually using:
```
#define SERIAL_BUFFER_SIZE 64
```
This is set in ...\arduino\hardware\arduino\avr\cores\arduino\HardwareSerial.h

Again, I would like to suggest you to check the power supply of esp first when run into any eccentric bug. The unsteadiness of
power will notably influence the performance of esp.

###Several Tips on Function Usage
1. No matter string[i] or char[i], it means the number ith character which **counts from 0**.
2. Serial.read() returns the first byte of incoming serial data available (or -1 if no data is available), while 
   Serial.readString reads characters from the serial buffer into a string and terminates if it times out. setTimeout function
   can set the maximum milliseconds to wait for serial data. If you know the terminator and the exact length of package, you 
   can also try *stream*.readBytes or *stream*.readBytesUntil.
   You should be aware that the parentheses **cannot** be omited.
3. string.indexOf ("...") returns the index of target characters "..." in a **string**. This also counts from **zero**.
   string.lastIndexOf ("...") find the target from the end of the string but the index still counts from the beginning.
4. Declare a hexadecimal integer: 
   int n=0x....;
   prefix 0x indicates that this is hex. And convert it to string in hex form:
   String str_hex=String (n,HEX);
5. The for loop must exist in a function.

###Things Could Be Done in Future
Plan to set the esp as a client and a concentrator, to get packages from wifi mesh upon request, decapsulate the packages and 
upload sensor data to websites like ThingSpeak for visualization.
On the other hand, esp can also be set as a webserver and a collector, to get data packages directly from sensor upon request
and upload them from certain ports and IP after decapsulation.
How to set the esp as client or webserver, you can check on my other posts in this repository.
Have fun!!
