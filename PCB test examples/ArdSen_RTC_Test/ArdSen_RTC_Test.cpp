/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

/*
 RTC DS3234 test on ArduSense
 	
 The circuit:
 * RTC attached to SPI bus as follows:
 ** CS - pin 6 on ArduSense as in Pro or Pro Mini (PD6, uC pin 10)
 ** MOSI - pin 11 on ArduSense as in Pro or Pro Mini (PB3, uC pin 15)
 ** MISO - pin 12 on ArduSense as in Pro or Pro Mini (PB4, uC pin 16)
 ** CLK - pin 13 on ArduSense as in Pro or Pro Mini (PB5, uC pin 17)
 
 Based on DeadOn RTC - DS3234 Breakout example Sketch
 Available from: https://www.sparkfun.com/products/10160
 Modified 3 February 2014 for ArduSense
 by Joaquín Vaquero
 
 */

#include <SPI.h>
//Beginning of Auto generated function prototypes by Atmel Studio
int RTC_init();
int SetTimeDate(int d, int mo, int y, int h, int mi, int s);
String ReadTimeDate();
//End of Auto generated function prototypes by Atmel Studio



// RTC CS signal. ArduSense: pin 6
const int RTC_chipselect = 6;  
// uSD CS signal. ArduSense: pin 10  
// This is PB2/SS (uC pin 14) 
// Note that even if it's not used as the CS pin, the hardware SS pin 
// must be left as an output or the SPI library functions will not work.
const int SD_chipSelect = 10;

// SPI Master SCK signal at pin 13 (PB5, pin 17 of ATmega328P)
// Output from the uController. RTC as a Slave
// Led attached to this pin
const int SCK_led = 13;
// RTC  PD2/INT* signal. ArduSense: pin 2
// PD2, uC pin 32
const int RTC_INT = 2;

// SD_EN signal. ArduSense: pin 4  PD4, digital pin 
const int SD_Sen_powerEnable = 4;  
// BEE_EN signal. ArduSense: pin 5  PD5, digital pin 
const int BEE_powerEnable = 5;

void setup() {
  Serial.begin(115200);
  
  // Configures the power enable control signals as outputs
  pinMode(SD_Sen_powerEnable, OUTPUT);
  pinMode(BEE_powerEnable, OUTPUT);   
  // Enables the power to SD, sensors nad communications modules
  digitalWrite(SD_Sen_powerEnable, HIGH);  
  digitalWrite(BEE_powerEnable, HIGH);    
  
    // uSD CS signal. ArduSense: pin 10  
  // This is PB2/SS (uC pin 14)// Note that even if it's not used as the CS pin,
  // the hardware SS pin 
  // must be left as an output or the SD library functions will not work. 
  pinMode(SD_chipSelect, OUTPUT);     // change this to 53 on a mega

  RTC_init();  //Initialites RTC DS3234
  // RTC DS3234 time setup. 
  // Day(1-31), month(1-12), year(0-99), hour(0-23), minute(0-59), second(0-59)
  // Day of the week not used.
  SetTimeDate(6,2,14,13,15,16); 
}

void loop() {
  Serial.println(ReadTimeDate());
  delay(1000);               // wait for a second
 
}

// ======== RTC DS 32342 initialization function =====================================
int RTC_init(){ 
  // RTC CS set as an output by default.
  pinMode(RTC_chipselect,OUTPUT);
  // PD2/INT* uC pin set as an input
  // this signal can be a square wave or an INT* signal
  // from RTC, depending on the RTC Control Register
  // configuration.
  pinMode(RTC_INT,INPUT);

  // Start the SPI library:
  SPI.begin();
  // Sets the MSB order for SPI
  SPI.setBitOrder(MSBFIRST); 
  SPI.setDataMode(SPI_MODE1); // Both modes 1 & 3 should work with RTC DS3234. 
                              // Reported problems on Forums with MODE1. We didn`t
                              // have any.
  
  SPI.setClockDivider(SPI_CLOCK_DIV2); // Sets the SPI clock divider relative to the system clock. 
  // Avilable dividers are 2, 4, 8, 16, 32, 64 or 128. 
  // The default setting is SPI_CLOCK_DIV4 sets the SPI clock to one-quarter the frequency 
  // of the system clock (4 Mhz for the boards at 16 MHz).
  // ArduSense has 8 MHz clock, and RTC supports up 4 MHz
  // Changed to SPI_CLOCK_DIV2 to get RTC SPI full speed.
  
  // Set Control Register 
  digitalWrite(RTC_chipselect, LOW);  // Selects RTC
  SPI.transfer(0x8E); // Control Register Address
  // Bit 7:EOSC*=1 Disables Oscillator when battery powered; 
  // Bit 6:BBSQW=1 Enables Battery SQ wave output on INT*/SQW pin;
  // Bit 5:CONV=0 Disable Temperature compensation; 
  // Bit 4-3:RS2-RS1=0-0 Output square wave on INT*/SQW pin at 1Hz; 
  // Bit 2:INTCN=0 Interrupt disabled; 
  // Bit 1-0:A2IE-A1IE=0-0 Alarmans Disabled; 
  // Control Register content=0x60
  SPI.transfer(0x60);
  digitalWrite(RTC_chipselect, HIGH); // Deselects RTC
  delay(10);
}

// ========= RTC DS 32342 Time setup function =========================================
// Day(1-31), month(1-12), year(0-99), hour(0-23), minute(0-59), second(0-59)
int SetTimeDate(int d, int mo, int y, int h, int mi, int s){ 
  int TimeDate [7]={
    s,mi,h,0,d,mo,y  }; // Day of the week not used, set to 0
  for(int i=0; i<=6;i++){
    if(i==3) // Day of the week not used, skipped
      i++;
    int b= TimeDate[i]/10;   // b keeps decens.
    int a= TimeDate[i]-b*10; // a keeps units.
    if(i==2){        // Set de 24/12 Hours mode
      if (b==2)      // Set 24 hour mode
        b=B00000010;   
      else if (b==1) // Set 12 hour mode
        b=B00000001;
    }	
    TimeDate[i]= a+(b<<4);  // Adds units and decens

    digitalWrite(RTC_chipselect, LOW); // Selects RTC
    SPI.transfer(i+0x80); // Timekeeping Registers Address, write mode
    SPI.transfer(TimeDate[i]); // Timekeeping Registers content, write value        
    digitalWrite(RTC_chipselect, HIGH);  // Deselects RTC
  }
}

// ========= RTC DS 32342 Time Read function =========================================
String ReadTimeDate(){
  String temp;
  int TimeDate [7]; // Second,minute,hour,null,day,month,year
                    // Day of the week not used, set to 0		
  for(int i=0; i<=6;i++){
    if(i==3)  // Day of the week not used, skipped
      i++;
    digitalWrite(RTC_chipselect, LOW); // Selects RTC
    SPI.transfer(i+0x00); // Timekeeping Registers Address, read mode
    unsigned int n = SPI.transfer(0x00); // Timekeeping Registers content, read value        
    digitalWrite(RTC_chipselect, HIGH); // Deselects RTC
    int a=n & B00001111;    // a keeps units.
    if(i==2){	
      int b=(n & B00110000)>>4; // Checks bit 5 of the hour register 0x02
      if(b==B00000010)          // If reads 1, 24 hour mode
        b=20;        
      else if(b==B00000001)     // If reads 0, 12 hour mode
        b=10;
      TimeDate[i]=a+b;          // Sets hour data according to the hour mode
    }
    else if(i==4){              // Sets decens for Days
      int b=(n & B00110000)>>4;
      TimeDate[i]=a+b*10;
    }
    else if(i==5){              // Sets decens for Months
      int b=(n & B00010000)>>4;
      TimeDate[i]=a+b*10;
    }
    else if(i==6){              // Sets decens for Years
      int b=(n & B11110000)>>4;
      TimeDate[i]=a+b*10;
    }
    else{	                // Sets decens for Minutes and Seconds
      int b=(n & B01110000)>>4;
      TimeDate[i]=a+b*10;	
    }
  }
  
  //Set time string format as dd/mm/yyyy  hh:mm:ss
  
  if (TimeDate[4]<10) {      // Includes 0 placeholders for single digit values
      temp.concat("0"); }  
  temp.concat(TimeDate[4]);
  temp.concat("/") ;
  if (TimeDate[5]<10) {      // Includes 0 placeholders for single digit values
      temp.concat("0"); }  
  temp.concat(TimeDate[5]);
  temp.concat("/") ;
  temp.concat("20");
  temp.concat(TimeDate[6]);
  temp.concat("     ") ;
 if (TimeDate[2]<10) {      // Includes 0 placeholders for single digit values
      temp.concat("0"); }  
  temp.concat(TimeDate[2]);
  temp.concat(":") ;
 if (TimeDate[1]<10) {      // Includes 0 placeholders for single digit values
      temp.concat("0"); }  
  temp.concat(TimeDate[1]);
  temp.concat(":") ;
 if (TimeDate[0]<10) {      // Includes 0 placeholders for single digit values
      temp.concat("0"); }  
  temp.concat(TimeDate[0]);
  return(temp);
}

