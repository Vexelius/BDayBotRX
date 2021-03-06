/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"
#include <printf.h>


/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

struct dataStruct {
  unsigned long timeCounter;  // Save response times
  char keyPress;          // When a key is pressed, this variable stores its unique code
  boolean keypadLock;     // When this flag is active, no input will be received fron the keypad
  boolean configMode;     // This flag determines wheter the robot is in Config Mode or not
  boolean statusDizzy;
} myData;                 // Data stream that will be sent to the robot


void setup() {
  Serial.begin(57600);
  printf_begin();
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
 radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setCRCLength(RF24_CRC_16);
  radio.setRetries(15,15);
  radio.setPayloadSize(8);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

  
  // Start the radio listening for data
  radio.startListening();
  radio.printDetails();
}

 
void loop()
{

  if ( radio.available())
  {

    while (radio.available())   // While there is data ready to be retrieved from the receive pipe
    {
      radio.read( &myData, sizeof(myData) );             // Get the data
    }

    radio.stopListening();                               // First, stop listening so we can transmit
    radio.write( &myData, sizeof(myData) );              // Send the received data back.
    radio.startListening();                              // Now, resume listening so we catch the next packets.

    Serial.print("Key Pressed: ");
    Serial.println(myData.keyPress);

    if(myData.keyPress == 'M')
    {
      Serial.println("Playing Melody!!!");
    }

    if(myData.configMode == true)
    {
      Serial.println("ConfigMode : Enabled");
    }

    if(myData.statusDizzy == true)
    {
      if(myData.keyPress == 'L')
      Serial.println("I'm Dizzy!!! (@_@ l)");
      if(myData.keyPress == 'R')
      Serial.println("I'm Dizzy!!! (r @_@)");
    }


    if(myData.statusDizzy == false)
    {
      Serial.println("I'm OK...");
    }


  } // END radio available

  
}//--(end main loop )---

