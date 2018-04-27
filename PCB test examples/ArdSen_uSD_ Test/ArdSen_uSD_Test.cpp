/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

/*
 uSD card test on ArduSense
 
 This example shows how use the utility libraries on which the
 uSD library is based in order to get info about the SD card.
 Useful for testing that the uSD card works.
 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** CS - pin 10 on ArduSense as in Pro or Pro Mini (PB2, uC pin 14)
 ** MOSI - pin 11 on ArduSense as in Pro or Pro Mini (PB3, uC pin 15)
 ** MISO - pin 12 on ArduSense as in Pro or Pro Mini (PB4, uC pin 16)
 ** CLK - pin 13 on ArduSense as in Pro or Pro Mini (PB5, uC pin 17)
 
 Created 28 Mar 2011
 by Limor Fried 
 Modified 9 Apr 2012
 by Tom Igoe
 Modified 24 January 2014 for ArduSense
 by Joaquín Vaquero
 */

// Include the SD library:
#include <SD.h>
// Include the SPI library if the SPI mode 
// is going to be changed:
//#include <SPI.h>
//Beginning of Auto generated function prototypes by Atmel Studio
void loop(void );
//End of Auto generated function prototypes by Atmel Studio



// Set up variables using the SD utility library functions:
// Construct an instance of Sd2Card, SdVolume and SdFile
Sd2Card card;
SdVolume volume;
SdFile root;

// uSD CS signal. ArduSense: pin 10
const int SD_chipSelect = 10;
// SPI Master SCK signal at pin 13 (PB5, pin 17 of ATmega328P)
// Output from the uController. uSD card as a Slave
// RTC CS signal. ArduSense: pin 6
const int RTC_chipSelect = 6;  
// Led attached to this pin
const int SCK_led = 13;
//boolean cd=false;

// SD_EN signal. ArduSense: pin 4  PD4, digital pin 
const int SD_Sen_powerEnable = 4;  
// BEE_EN signal. ArduSense: pin 5  PD5, digital pin 
const int BEE_powerEnable = 5;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  pinMode(SCK_led,OUTPUT);
  // SPI mode 0 for uSD. Check if used with another SPI device
  // such as a RTC that may use a different SPI mode.
  // To use this function, include SPI.h library.
  //SPI.setDataMode(SPI_MODE0); 
  
  // Configures the power enable control signals as outputs
  pinMode(SD_Sen_powerEnable, OUTPUT);
  pinMode(BEE_powerEnable, OUTPUT);   
  // Enables the power to SD, sensors nad communications modules
  digitalWrite(SD_Sen_powerEnable, HIGH);  
  digitalWrite(BEE_powerEnable, HIGH);    
  
  // uSD CS set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // must be left as an output or the SD library functions will not work. 
  pinMode(SD_chipSelect, OUTPUT);     // change this to 53 on a Mega
  // RTC CS set as an output by default.
  pinMode(RTC_chipSelect, OUTPUT);     
  digitalWrite(RTC_chipSelect, HIGH);   // Disables RTC to avoid SPI bus collisions
  delay(4000);
  // see if the card is present and can be initialized:
  if (!SD.begin(SD_chipSelect)) {
    Serial.println("\nCard failed, or not present");
    return;
  }
}

void loop(void) {
  Serial.print("\nInitializing SD card...");
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, SD_chipSelect)) {
    Serial.println(" initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the SD_chipSelect pin to match your shield or module?");
    delay(2000);
    return;
  } 
  else {
    Serial.println("Wiring is correct and a card is present."); 
  }

  // print the type of card
  Serial.print("\nCard type: ");
  switch(card.type()) {
  case SD_CARD_TYPE_SD1:
    Serial.println("SD1");
    break;
  case SD_CARD_TYPE_SD2:
    Serial.println("SD2");
    break;
  case SD_CARD_TYPE_SDHC:
    Serial.println("SDHC");
    break;
  default:
    Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    delay(2000);
    return;
  }

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                         // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
  delay(2000);
}

