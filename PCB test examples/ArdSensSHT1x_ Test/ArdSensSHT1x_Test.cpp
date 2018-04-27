/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

/**
 * ReadSHT1xValues
 *
 * Read temperature and humidity values from an SHT1x-series (SHT10,
 * SHT11, SHT15) sensor.
 *
 * Copyright 2009 Jonathan Oxer <jon@oxer.com.au>
 * www.practicalarduino.com
 */

/* Modified 13 February 2014 for ArduSense
   by Joaquín Vaquero
   Electronic Technology Department, Universidad Rey Juan Carlos. Spain.
   */

   
#include <SHT1x.h>
//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio



// Specify data and clock connections and instantiate SHT1x object for Relative Humidity and Temperature sensor
#define dataPin1  8  // DATA1 - pin 8 on ArduSense as in Pro (PB0, ATmega328 pin 12)
#define clockPin1 7  // SCK2 - pin 7 on ArduSense as in Pro (PD7, ATmega328 pin 11)
SHT1x sht1x1(dataPin1, clockPin1);  // Values for Clock and Data pin to the SHT1x class


// Specify data and clock connections and instantiate SHT1x object for Soil Moisture and Temperature sensor
#define dataPin2  A1  // DATA2 - pin A1 on ArduSense as in Pro (PC1, ATmega328 pin 24)
#define clockPin2 A0  // SCK2 - pin A0 on ArduSense as in Pro (PC0, ATmega328 pin 23)
SHT1x sht1x2(dataPin2, clockPin2);  // Values for Clock and Data pin to the SHT1x class

// Specify data and clock connections and instantiate SHT1x object for Soil Moisture and Temperature sensor
#define ch_status  A6  // CH_SATATU - pin A6 on ArduSense ADC6, ATmega328 pin 19)
#define lipo_vol A7    // LIPO_Vol - pin A7 on ArduSense ADC7, ATmega328 pin 22)

// SD_EN signal. ArduSense: pin 4  PD4, digital pin 
const int SD_Sen_powerEnable = 4;  
// BEE_EN signal. ArduSense: pin 5  PD5, digital pin 
const int BEE_powerEnable = 5;  

void setup()
{
  Serial.begin(115200); // Open serial connection to report values to host
  
  // Configures the power enable control signals as outputs
  pinMode(SD_Sen_powerEnable, OUTPUT);
  pinMode(BEE_powerEnable, OUTPUT);   
  // Enables the power to SD, sensors nad communications modules
  digitalWrite(SD_Sen_powerEnable, HIGH);  
  digitalWrite(BEE_powerEnable, HIGH);  
  
  // Configures the ADC channels ADC6 and ADC7 as analog inputs
  pinMode(ch_status, INPUT);
  pinMode(lipo_vol, INPUT); 
  
  Serial.println("Starting up");
  delay(11);   // Waiting for sensor start up. Needs 11ms to
  // get to Sleep State. No commands must be sent before
  // that time.
}

void loop()
{
  float temp_c1;     // Temperature value, 14 bits resolution, RH & T sensor
  float humidity1;   // Compensated humidity value, 12 bits resolution, RH & T sensor
  float temp_c2;     // Temperature value, 14 bits resolution, SM & T sensor
  float humidity2;   // Compensated humidity value, 12 bits resolution, SM & T sensor

  // Read values from both sensors
  temp_c1 = sht1x1.readTemperatureC();
  humidity1 = sht1x1.readHumidity();
  temp_c2 = sht1x2.readTemperatureC();
  humidity2 = sht1x2.readHumidity();

  // Print the values to the serial port
  Serial.print("Temperature RH & T: ");
  Serial.print(temp_c1, DEC);
  Serial.print("C / Relative Humidity: ");
  Serial.print(humidity1);
  Serial.println("%");
  Serial.print("Temperature SM & T: ");
  Serial.print(temp_c2, DEC);
  Serial.print("C / Soil Moisture: ");
  Serial.print(humidity2);
  Serial.println("%");

  delay(5000);
}
