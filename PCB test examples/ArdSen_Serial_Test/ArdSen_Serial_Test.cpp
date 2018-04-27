/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */


//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio

/*
 ArdSen_Serial_Test
  Test serial communications with Serial monitor.
  Includes power-enable for the uSD and for the sensors.
  Includes power-enable for the communications module.
  
  The circuit:
  SD_EN - pin 4 on ArduSense as in Pro (PD4, ATmega328 pin 2)
  BEE_EN - pin 5 on ArduSense as in Pro (PD5, ATmega328 pin 9)
  
 This example code is in the public domain.
 */
int testdata = 0;   // data to be sent
int incdata = 5;    // data value increment, to send different data each time

// SD_EN signal. ArduSense: pin 4  PD4, digital pin 
const int SD_Sen_powerEnable = 4;  
// BEE_EN signal. ArduSense: pin 5  PD5, digital pin 
const int BEE_powerEnable = 5;  

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  // Only for tests, 9600 baud
  //Serial.begin(9600);
  // Configures the power enable control signals as outputs
  pinMode(SD_Sen_powerEnable, OUTPUT);
  pinMode(BEE_powerEnable, OUTPUT);   
  // Enables the power to SD, sensors and communications modules
  digitalWrite(SD_Sen_powerEnable, HIGH);  
  digitalWrite(BEE_powerEnable, HIGH);  
}

// the loop routine runs over and over again forever:
void loop() {
  
  // change the number for next time through the loop:
  testdata = testdata + incdata;

  // reverse the increment of the data value 
  if (testdata == 0 || testdata == 255) {
    incdata = -incdata ; 
    // Disables the power to SD, sensors and communications modules
    digitalWrite(SD_Sen_powerEnable, LOW);  
    digitalWrite(BEE_powerEnable, LOW);  
  }     
                             
  Serial.println(testdata);
  // Wait 300 milliseconds before sending next data    
  delay(300); 
  // Enables the power to SD, sensors nad communications modules
  digitalWrite(SD_Sen_powerEnable, HIGH);  
  digitalWrite(BEE_powerEnable, HIGH);  
}  

