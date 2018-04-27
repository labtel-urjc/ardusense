/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */


//Beginning of Auto generated function prototypes by Atmel Studio
float map_float(int CAD_counts, int min_counts, int max_counts, float out_min, float out_max);
unsigned char read_charger_status(int Charger_status_counts);
//End of Auto generated function prototypes by Atmel Studio

/*
 ArdSen_Bat_Test
  Test Bat level and charger status and shows values with Serial monitor.
  Includes power-enable for the uSD and for the sensors.
  Includes power-enable for the communications module.
  
  The circuit:
  SD_EN - pin 4 on ArduSense as in Pro (PD4, ATmega328 pin 2)
  BEE_EN - pin 5 on ArduSense as in Pro (PD5, ATmega328 pin 9)
  
 This example code is in the public domain.
 */

int Batt_counts = 0;   // Battery voltage level, in CAD counts
float Batt_volt = 0;     // Battery voltage level, in volts (V)
int Batt_status_counts;   // Battery charger status, in CAD counts

// SD_EN signal. ArduSense: pin 4  PD4, digital pin 
const int SD_Sen_powerEnable = 4;  
// BEE_EN signal. ArduSense: pin 5  PD5, digital pin 
const int BEE_powerEnable = 5;  
// LIPO_VOL analog value, for the battery voltage level. ArduSense: pin 7, ananlog input
const int LIPO_VOL_value = A7;
// Battery charger status values. ArduSense: pin 6, ananlog input
const int BATT_status = A6;
// Battery charger status strings array
char* CH_status_print[]=
  {
    "Off","Charging","OK","Error"
  };
// Battery charger status variable
unsigned char CHstatus = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  // Only for tests, 9600 baud
  //Serial.begin(9600);
  // Configures the power enable control signals as outputs
  pinMode(SD_Sen_powerEnable, OUTPUT);
  pinMode(BEE_powerEnable, OUTPUT);   
  // Enables the power to SD, sensors nad communications modules
  digitalWrite(SD_Sen_powerEnable, HIGH);  
  digitalWrite(BEE_powerEnable, HIGH);  
  // Configures the reference voltage for the analog input equal to 1.1 volts
  // Maximum input voltage to CAD in ArduSense is 0.7 V (Battery full load = 4.2V)
  analogReference(INTERNAL);
}

// the loop routine runs over and over again forever:
void loop() {
  
  // Reads the battery voltage
  Batt_counts = analogRead(LIPO_VOL_value);    // Read the battery voltage, in counts
  delayMicroseconds(150);        // Pauses for 150 microseconds for CAD stabilization. Minimum 100us
  // Reads the battery charger status
  Batt_status_counts = analogRead(BATT_status);    // Read the battery voltage, in counts
  
  Serial.println(Batt_counts);                 // Print the the battery voltage, in counts
  Batt_volt = map_float(Batt_counts,0,1023,0,6.6); // 
  // Print the the battery voltage, in volts (V)
  Serial.print("Tension de la bateria: ");
  Serial.print(Batt_volt);      
  Serial.println(" V");
  
  CHstatus = read_charger_status(Batt_status_counts); // Read the charger status
  Serial.println(Batt_status_counts);
  Serial.print("Carga de bateria: ");
  Serial.println(CH_status_print[CHstatus]); 
  
  // Wait 300 milliseconds before sending next data 
  delay(300); 
  // Enables (HIGH) disables (LOW) the power to SD, sensors and communications modules
  //digitalWrite(SD_Sen_powerEnable, HIGH);  
  //digitalWrite(BEE_powerEnable, HIGH);  
}  


// ======== Converts CAD counts to real values in double format ===================================
// Similar to map()

float map_float(int CAD_counts, int min_counts, int max_counts, float out_min, float out_max)
  { 
  return (CAD_counts - min_counts) * (out_max - out_min) / (max_counts - min_counts) + out_min;
  }
  
  
// ======== Charger status read ===================================
  
 unsigned char read_charger_status(int Charger_status_counts)
 {
  unsigned char CH_Status=0;
  if(Charger_status_counts>900)
  {
    CH_Status = 0; // sleeping
  }
  else if(Charger_status_counts>550)
  {
    CH_Status = 1;// charging
  }
  else if(Charger_status_counts>350)
  {
    CH_Status = 2;// Charged
  }
  else
  {
    CH_Status = 3;//error
  }
  return CH_Status;
}

    
    
    
    
