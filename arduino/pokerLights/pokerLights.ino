/*
 Poker Table Light Control
                                                                                                                               
 Justin Trzeciak
 
 Last Updated 04JANUARY2013
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 Networking based on DHCP Chat Server example by Tom Igoe
 Lighting based on strandTest in LPD8806 library
 Horn based on melody by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <LPD8806.h>
#include "pitches.h"

//Set up pins
int dataPin = 2;    //DI on Lights
int clockPin = 3;   //CI on Lights
int manualPin = 5;	//Toggle Switch for Manual Mode
int setPin = 6;		//Toggle Switch for Set
int pushButton = 7; //Button on front of case
int greenLed = 8;	//Green LED on front of case
int redLed = 9; 	//Red LED on front of case
int locPin = A0;    //Potentiometer input to represent seat number.
int colPin = A1;    //Potentiometer input to represent color to set.
int bPin = A2;      //Potentiometer input to represent desired brightness.
int progPin = A3;	//Potentiometer input to represent desired program.
int hornPin = A4;   //Internal Speaker
int ledCount = 96;  //Number of LEDs on Strip

// Set the first variable to the NUMBER of pixels. 32 = 32 pixels in a row
// The LED strips are 32 LEDs per meter but you can extend/cut the strip
// I have 3 meters.
LPD8806 strip = LPD8806(ledCount, dataPin, clockPin);

// Enter a MAC address and IP address for your controller below.
// Also enter a port for the controller to listen on. 
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x18, 0x4B };
IPAddress ip(192,168,2,100);
IPAddress gateway(192,168,2, 1);
IPAddress subnet(255, 255, 255, 0);
int port = 1234;

EthernetServer server(port);
boolean gotAMessage = false; // whether or not you got a message from the web page yet

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // this check is only needed on the Leonardo:
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection:
  Serial.println("Trying to get assigned IP address");
  Ethernet.begin(mac, ip);
  // print your local IP address:
  Serial.print("My IP address: ");
  //ip = Ethernet.localIP();
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
  
  // initialize the I/O pins:
  pinMode(pushButton, INPUT);    
  pinMode(manualPin, INPUT);
  pinMode(setPin, INPUT);       
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(hornPin, OUTPUT);  
}

void loop() {
  
  int i;
  char buffer[54];
  
  digitalWrite(greenLed, HIGH);			//Simply indicates the controller is on
  digitalWrite(redLed, LOW);			//Only turns on when manual mode is set
  
  if(digitalRead(pushButton) == HIGH)	//Activate the horn when the pushbutton is pressed
    horn();
  
  // wait for a message from the web page:
  EthernetClient client = server.available();
	  
  // when the sends the first byte, say hello:
  if (client)
  {
    if (!gotAMessage) 
    {
      Serial.println("Initial message received from web page.");
      gotAMessage = true;
    }
    for (i = 0; i < 54; i++)
    {
      // read the bytes incoming from the client:
      char thisChar = client.read();
      // echo the bytes to the server as well:
      Serial.print(thisChar);
      buffer[i] = thisChar;
    }
    Serial.println();
    
    //Check to see the packet is complete. Make sure it starts with "?", is 14 bytes long
    //and ends with a "!".  
    if ((buffer[0] != '?') || (buffer[53] != '!'))
      Serial.println("Error receiving packet!");
    else
      Serial.println(buffer);
  }
  
  //If manual mode is set on the remote, bypass the
  //network activity and call the manual function.
  if (digitalRead(manualPin) == HIGH)
  {
    for (i = 0; i < 54; i++)
    {
	  //Clear out the any incoming data from the web page. It won't be used.
      buffer[i] = 0;
      manualMode();
    }
  }
  else
  {
    // Pass the buffer to the lights function.
    // The function will interpret the data and change the lights accordingly.
    autoMode(buffer);
  }
}

//This function handles the data packet from the web page
//when manual mode is not active.
void autoMode(char *buff)
{
  int i;
  //Check to see the packet is complete. Make sure it starts with "?", is 14 bytes long
  //and ends with a "!".  
  if ((buff[0] != '?') || (buff[53] != '!'))
    return;
  else
  {
  	//Convert the brightness string to an integer value
    char brightString[4];
    brightString[0] = buff[2];
    brightString[1] = buff[3];
    brightString[2] = buff[4];
    brightString[3] = 0;
    int autoBright = atoi(brightString);
   
    //Take the character for the program value and make it a string so we can use atoi
    char progString[2];
    progString[0] = buff[1];
    progString[1] = 0;
    //Convert the program value from a string to an integer value
    int autoProg = atoi(progString);
    
    //Convert the hex color strings to numbers
    char colorStr[8][3][2];    //[Seat Color][Red, Green, or Blue][MSB or LSB]
    int colors[8][3];          //[Seat Color][Red, Green, or Blue]
    int j, k;
    int l = 5;
    //Place all the characters from buff into their respective places in colorStr.
    for (i = 0; i < 8; i++)
    {
    	for (j = 0; j < 3; j++)
    	{
    		for (k = 0; k < 2; k++)
    		{
    			colorStr[i][j][k] = buff[l];
    			l++;
    		}
    	}
    }
    //Convert the hex characters to actual numbers. Place them in colors[][].
    for (i = 0; i < 8; i++)
    {
    	for (j = 0; j < 3; j++)
        {
    		colors[i][j] = hextoi(colorStr[i][j])/2;
        }
    }
  	
  	//If no program is selected, set the colors as indicated by the packet from the web page  
    if (autoProg == 0)
    {
  	for (i = 0; i < 8; i++)
  		colorSelect(i, colors[i]);
    }
    else
    //Otherwise, run the appropriate program
  	runProg(autoProg, autoBright);
  }
}


//When the toggle switch is set to manual mode,
//This functions displays the colors based on analog inputs.
void manualMode()
{
  digitalWrite(redLed, HIGH);	//Indicate manual mode is active
  
  int i, r, g, b;	//Set up counter and color values
  
  //Partition the location potentiometer into the correct amount of locations
  int totalSeats = 8;
  double locScale = totalSeats/1024.0;
  //Figure out what seat is selected, store as int
  double location = locScale * analogRead(locPin);
  int seat = (int)location;
  
  //Rescale the brightness potentiometer from 1024 to 127
  double bScale = 127/1024.0;	//Must use 1024.0 since the answer is a fraction
  //Convert the brightness to an integer
  double brightness_dbl = bScale * analogRead(bPin);
  int brightness = (int)brightness_dbl;

  //Partition the color potentiometer into the correct amount of colors 
  int totalColors = 384;
  double colScale = totalColors/1024.0;
  //Figure out what color is selected, store as int
  double setColor_dbl = colScale * analogRead(colPin);
  int setColor = (int)setColor_dbl;
  
  //Partition the program potentiometer into the correct amount of programs
  int totalProgs = 6;
  double progScale = totalProgs/1024.0;
  //Figure out what program is selected, store as int
  double setProg_dbl = progScale * analogRead(progPin);
  int setProg = (int)setProg_dbl;

  //If a program is selected, run it.
  if(setProg != 0)
  {
  	//Only set the new values when the toggle switch is "on"
    if(digitalRead(setPin) == HIGH)		
      runProg(setProg, brightness);
  }
  //If no program is selected, set the colors as indicated by the potentiometers
  else
  {       
          uint32_t color = Wheel(setColor, brightness); //Assign the color to the strip
	  
	  //Only set the new values when the toggle switch is "on"
	  if(digitalRead(setPin) == HIGH)
	  {
		for (i=0; i < 12; i++)		//There are 12 lights on the strip per seat
			//Set selected color to the appropriate location
			strip.setPixelColor((seat * 12) + i, color);
	  }
	  strip.show();				//Finally apply the changes
	}
}

//Pick which program to run
//The first four came with the library for the lights
//I made the fifth, as well as adapting the existing for a brightness parameter.
void runProg(char index, int brightness)
{
  if(index == 1)
  {
    // Send a simple pixel chase in...
    colorChase(strip.Color(brightness,0,0), 10);  		// red
    colorChase(strip.Color(brightness,brightness,0), 10);	// orange
    colorChase(strip.Color(0,brightness,0), 10);			// green
    colorChase(strip.Color(0,brightness,brightness), 10);	// teal
    colorChase(strip.Color(0,0,brightness), 10);			// blue
    colorChase(strip.Color(brightness,0,brightness), 10);	// magenta
    
    int i;  
    for (i=0; i < strip.numPixels(); i++)
      strip.setPixelColor(i, 0);  // turn all pixels off
  }
  else if (index == 2)
  {
	  colorWipe(strip.Color(brightness,0,0), 10);		// red
	  colorWipe(strip.Color(0, brightness,0), 10);		// green
	  colorWipe(strip.Color(0,0,brightness), 10);		// blue
  }
  else if (index == 3)
	rainbow(10, brightness);
  else if (index == 4)
	rainbowCycle(0, brightness);  // make it go through the cycle fairly fast
  else if (index == 5)
	fade(10, brightness);
  else 
	Serial.println("Unrecognized program code.");
}

//This function manually sets the colors based on the web page
//It is very similar to the manual color select function with a few differences
//For instance, we don't have to worry about the set switch anymore
void colorSelect(int seat, int color[])
{
	int i;
  		
	uint32_t stripColor = strip.Color(color[0],color[1],color[2]);	//Assign the color to the strip
	
	for (i=0; i < 12; i++)		//There are 12 lights on the strip per seat
		//Set selected color to the appropriate location
		strip.setPixelColor((seat * 12) + i, stripColor);
	
	strip.show();  				//Finally apply the changes
}

//Below are the functions for the light programs

void fade(uint8_t wait, int brightness) {
  int i, j;
   
  for (j=0; j < 384; j++) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(j % 384, brightness));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}


//The last 5 were included in the LPD8806 library

void rainbow(uint8_t wait, int brightness) {
  int i, j;
   
  for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 384, brightness));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait, int brightness) {
  uint16_t i, j;
  
  for (j=0; j < 384; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384, brightness) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Chase a dot down the strip
// good for testing purposes
void colorChase(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);  // turn all pixels off
  } 
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      if (i == 0) { 
        strip.setPixelColor(strip.numPixels()-1, 0);
      } else {
        strip.setPixelColor(i-1, 0);
      }
      strip.show();
      delay(wait);
  }
}

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r
//Adapted by me for brightness

uint32_t Wheel(uint16_t WheelPos, int wheelBright)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  
  //Scale for brightness
  r = r * wheelBright/127;
  g = g * wheelBright/127;
  b = b * wheelBright/127;
  
  return(strip.Color(r,g,b));
}

//Function for horn from Tom Igoe's program
void horn()
{
  // notes in the melody:
  int melody[] = {
  NOTE_A4, NOTE_E4,NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_E4};
  
  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {16,16,8,8,16,16,16,16,8,8,8,8 };  
  
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 12; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 2400/noteDurations[thisNote];
    tone(hornPin, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(hornPin);
  }
}

//Function to convert a two digit hex number string to an integer.
//[1] is the LSB and [0] is the MSB. Accepts lower or upper case.
int	hextoi(char hex[])
{
	int num = 0;
	if ((hex[1]>='0')&&(hex[1]<='9'))
		num += (hex[1]-'0');
	if ((hex[1]>='a')&&(hex[1]<='f'))
		num += (hex[1]-'a');
	if ((hex[1]>='A')&&(hex[1]<='F'))
		num += (hex[1]-'A');
	if ((hex[0]>='0')&&(hex[0]<='9'))
		num += (hex[0]-'0')*16;
	if ((hex[0]>='a')&&(hex[0]<='f'))
		num += (hex[0]-'a'+10)*16;
	if ((hex[0]>='A')&&(hex[0]<='F'))
		num += (hex[0]-'A'+10)*16;
        return num;
}
