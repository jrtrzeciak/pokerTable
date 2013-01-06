/*
 Poker Table Light Control
 
 Justin Trzeciak
 
 Last Updated 20JUNE2012
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 Networking based on DHCP Chat Server example by Tom Igoe
 Lighting based on strandTest in LPD8806 library
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <LPD8806.h>

//Set up pins
int dataPin = 2;    //DI on Lights
int clockPin = 3;   //CI on Lights
int ledCount = 96;  //Number of LEDs on Strip
int locPin = A0;    //Potentiometer input to represent seat number.
int colPin = A1;    //Potentiometer input to represent color to set.
int bPin = A2;      //Potentiometer input to represent desired brightness.

// Set the first variable to the NUMBER of pixels. 32 = 32 pixels in a row
// The LED strips are 32 LEDs per meter but you can extend/cut the strip
// I have 3 meters.
LPD8806 strip = LPD8806(ledCount, dataPin, clockPin);

// Enter a MAC address and IP address for your controller below.
// Also enter a port for the controller to listen on. 
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x18, 0x4B };
IPAddress ip(192,168,2,100);
IPAddress gateway(192,168,2, 1);
IPAddress subnet(255, 255, 255, 0);
int port = 1234;

EthernetServer server(port);
boolean gotAMessage = false; // whether or not you got a message from the client yet

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // this check is only needed on the Leonardo:
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection:
  Serial.println("Trying to get assigned IP address");
  Ethernet.begin(mac, ip, gateway, subnet);
  // print your local IP address:
  Serial.print("My IP address: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(ip[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
  // start listening for clients
  server.begin();
  
  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
}

void loop() {
  
  int i;
  char buffer[3];
  
  // wait for a new client:
  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client) {
    if (!gotAMessage) {
      Serial.println("We have a new client");
      client.println("Hello, client!"); 
      gotAMessage = true;
    }
    for (i = 0; i < 3; i++)
    {
      // read the bytes incoming from the client:
      char thisChar = client.read();
      // echo the bytes to the server as well:
      Serial.print(thisChar);
      buffer[i] = thisChar;
    }
    //buffer[5] = 0;
    Serial.println();
    Serial.print(buffer);
    // Pass the buffer to the lights function.
    // The function will interpret the data and change the lights accordingly.
    lights(buffer);
  }
}

void lights(char *buff)
{
  int i, r, g, b, seat;
  double locScale = strip.numPixels()/1024.0;
  double location = locScale * analogRead(locPin);
  int loc = buff[1] - '0'; //int(location);
  double bScale = 127/1024.0;
  double brightness = buff[2] - '0'; //bScale * analogRead(bPin);

 
  if (buff[0] == 'b')
  {
    r = 0;
    b = brightness;
    g = 0;
  }
  
  if (buff[0] == 'r')
  {
    r = brightness;
    b = 0;
    g = 0;
  }
  
  if (buff[0] == 'g')
  {
    r = 0;
    b = 0;
    g = brightness;
  }
  
  /*for(i=0; i < 8; i++)
  {
    if((loc > i * 12) && (loc <= (i + 1) * 12))
      seat = i * 12;
  }*/
  
  seat = loc * 12;
  
  uint32_t color = strip.Color(r,g,b);
  
  for (i=0; i < strip.numPixels(); i++)
    strip.setPixelColor(i, 0);  // turn all pixels off
  
  for (i=0; i < 12; i++)
    strip.setPixelColor(seat + i, color);
  strip.show(); 
}  

