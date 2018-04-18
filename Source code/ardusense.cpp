/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
/*End of auto generated code by Atmel studio */

// Include libraries used
#include <config.h>
#include <SD.h>
#include <SPI.h>
#include <XBee.h>
#include <ds3234.h>
#include <Time.h>
#include <SHT1x.h>
#include <TimeLib.h>
#include <SnoozeLib.h>
//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio

/*
Modificar en la libreria SHT1x las siguientes lineas

En float SHT1x::readTemperatureC()

// Conversion coefficients from SHT15 datasheet
//const float D1 = -40.0;  // for 14 Bit @ 5V
const float D1 = -39.7;  // for 14 Bit @ 3.5V
const float D2 =   0.01; // for 14 Bit DEGC

En float SHT1x::readTemperatureF()

// Conversion coefficients from SHT15 datasheet
//const float D1 = -40.0;   // for 14 Bit @ 5V
const float D1 = -39.5;  // for 14 Bit @ 3.3V
const float D2 =   0.018; // for 14 Bit DEGF

*/

//Beginning of Auto generated function prototypes by Atmel Studio
void createlogfile();
void saveinSD();
void send_to_coord();
void sendData();
void getTH();
void gettimeRTC();
void readbatlevel();
void send_av_data();
void send_cmd_error();
void send_ack_endtx();
void send_sincro_rtc();
void send_time_data();
void send_file();
byte read_charger_status(byte Charger_status_counts);
void comprobar_RTC();
//End of Auto generated function prototypes by Atmel Studio

//------------------------------------------

// EndPoint_ID. Unique identifier for each mote

int ENDPOINT_ID = 1;
char SAN_Endpoint_Id[]="endp_1";

//int ENDPOINT_ID = 2;
//char SAN_Endpoint_Id[]="endp_4";

//int ENDPOINT_ID = 3;
//char SAN_Endpoint_Id[]="endp_5";

//int ENDPOINT_ID = 4;
//char SAN_Endpoint_Id[]="endp_3";

//int ENDPOINT_ID = 5;
//char SAN_Endpoint_Id[]="endp_6";

//------------------------------------------

// SD VARIABLES
// Construct an instance of Sd2Card, SdVolume and SdFile

char file_name_1[20]={};
char data_frame[60]={};
const int SD_chipSelect = 10;
const int SD_Sen_powerEnable = 4;
String file_no_send[5]={};
byte num_file=0;

//------------------------------------------

//Variables de control de tiempo dormido y env�o de datos

byte send_done=0;
unsigned long tiempo;
unsigned long tiempo_dormido;
byte Intervalo_toma_datos = 15; // Intervalo de tiempo en el que se despierta para tomar datos. Est� en minutos
boolean firstime = true;

//------------------------------------------

//Sensor variables

float datosTempCorregido_1=0;
float datosHumedadCorregido_1=0;
float datosTempCorregido_2=0;
float datosHumedadCorregido_2=0;
#define dataPin1  8  // DATA1 - pin 8 on ArduSense as in Pro (PB0, ATmega328 pin 12)
#define clockPin1 7  // SCK2 - pin 7 on ArduSense as in Pro (PD7, ATmega328 pin 11)
#define dataPin2 A1 // DATA2 - pin A1 on ArduSense as in Pro (PC1, ATmega328 pin 24)
#define clockPin2 A0 // SCK2 - pin A0 on ArduSense as in Pro (PC0, ATmega328 pin 23)



//------------------------------------------

//Battery status varibales

// Lipo Battery voltage

const int LIPO_VOL_value = A7;

// Battery charger status values. ArduSense: pin 6, analog input

const int BATT_status = A6;
int bat_level;
int CHstatus = 0;


//------------------------------------------

//RTC VAriables

const int RTC_chipSelect = 6;
const int RTC_INT = 2;
struct ts t_set;
struct ts t_ant;
time_t t;

//------------------------------------------

//Xbee comunication variables

const int BEE_powerEnable = 5;
const int BEE_sleep = 9;
XBeeAddress64 addr64_coord =  XBeeAddress64(0x0013a200, 0x40B1355D); // coordinador raspberry xbee-c
//XBeeAddress64 addr64_coord =  XBeeAddress64(0x0013a200, 0x40AD63A7); // coordinador xctu
XBee xbee = XBee();
DMRxResponse rxResponse = DMRxResponse();
DMTxRequest  tx_to_coord = DMTxRequest();

//------------------------------------------

//datos para reiniciar el micro

byte contador_ciclos = 0;
boolean hora_rtc = false;
//------------------------------------------

void setup()
{
	// Configures the reference voltage for the analog input equal to 1.1 volts
	// Maximum input voltage to CAD in ArduSense is 0.7 V (Battery full load = 4.2V)
	analogReference(INTERNAL);
	Serial.begin(9600);
	xbee.setSerial(Serial);
	pinMode(BEE_sleep,OUTPUT);
	digitalWrite(BEE_sleep,HIGH);
	pinMode(BEE_powerEnable,OUTPUT);
	digitalWrite(BEE_powerEnable, HIGH);
	pinMode(SD_Sen_powerEnable, OUTPUT);
	digitalWrite(SD_Sen_powerEnable, HIGH);
	pinMode(SD_chipSelect, OUTPUT);     // change this to 53 on a mega
	digitalWrite(SD_chipSelect, HIGH);
	pinMode(RTC_chipSelect, OUTPUT);
	digitalWrite(RTC_chipSelect, HIGH);
	setTime(01,01,01,01,01,2018);
	t=now();
	delay(3000);
	DMRxResponse rxResponse = DMRxResponse();
	SPI.setDataMode(SPI_MODE0);
	digitalWrite(SD_chipSelect, LOW);
	if (!SD.begin(SD_chipSelect))
	{
		Serial.println(F("Card failed, or not present"));
		return;
	}
	else
	{
		Serial.println("ok");
	}
	digitalWrite(SD_chipSelect, HIGH);
	
	
	
}

//------------------------------------------

void loop()
{
	comprobar_RTC(); // comprobamos si el reloj est� en hora o se ha reiniciado.
	
	if ((firstime == true) && (hora_rtc == true)) // si es un primer arranque pero el reloj est� en hora, cuando reseteamos el micro por software
	{
		firstime = false;
		gettimeRTC();
		createlogfile();
	}
	if ((firstime == true) && (hora_rtc == false)) // si es un primer arranque y el reloj no est� en hora, cuando arrancamos por primera vez o nos hemos quedado sin bater�a
	{
		send_to_coord();
	}
	else
	{
		
		readbatlevel();
		getTH();
		gettimeRTC();
		saveinSD();
		send_done = 0;
		
		// comprobamos si es hora de conectarse a la red y enviar datos
		
		// Conectamos a las 12 con el coordinador
		
		if ((hour(t) == 12) && (minute(t) == 00))
		{
			
			tiempo = millis();
			send_to_coord();
			t_ant=t_set;
		}
		
		if (contador_ciclos == 5) // cada 5 ciclos de trabajo realiamos una lectura de direcci�n de memoria 0 haciendo que se resetee el micro para liberar memoria.
		{
			asm("jmp 0x0000");
		}
		
		t=now(); // leemos la hora actual para saber cuando nos tenemos que despertar a tomar datos
		tiempo_dormido = ((Intervalo_toma_datos * 60) - (((minute(t)*60)+second(t)) % (Intervalo_toma_datos*60)));
		snoozeLib.snooze (tiempo_dormido * 1000);
	}
	
}

//------------------------------------------

void send_to_coord() // funcion de control de la comunicaci�n con el coordinador y del tiempo que est� despierto si no se comunican
{
	unsigned long tiempo_actual;
	digitalWrite(BEE_sleep,LOW);
	tiempo = millis();
	while (send_done != 1)
	{
		xbee.readPacket();
		if (xbee.getResponse().isAvailable())
		{
			if (xbee.getResponse().getApiId() == DM_RX_RESPONSE)
			{
				xbee.getResponse().getDMRxResponse(rxResponse);
				if (rxResponse.getData(0) == 0x10)
				{//envia el numero de ficheros que est�n sin enviar.
					send_av_data();
				}else if (rxResponse.getData(0) == 0x20)
				{ //envia los datos pendientes de envio.
					sendData();
				} else if (rxResponse.getData(0) == 0x30)
				{// recibe un ok del envio de datos y envia otro.
					num_file=0;
					memset (file_no_send,0,sizeof(file_no_send));
					send_ack_endtx();
				}else if (rxResponse.getData(0) == 0x40)
				{// sincroniza la hora con la del nodo coordinador.
					send_sincro_rtc();
				}
				else if (rxResponse.getData(0) == 0x50)
				{ // cambiamos la frecuencia con la que toma datos, nos la mandar�n en minutos.
					send_time_data();
				}else if (rxResponse.getData(0) == 0x60)
				{ // envia un fichero concreto que nos est�n pidiendo.
					send_file();
				}
				else
				{
					send_cmd_error();
				}
			}
		}
		if (firstime == false) // si es la primera vez no dormimos el equipo hasta que se sincronice con el coordinador si no es la primera vez creamos un fichero nuevo y dormimos el equipo a los 3 minutos de haberse conectado
		{
			tiempo_actual = millis();
			if (tiempo_actual > (tiempo + 180000))
			{//transcurridos x segundos o minutos se apaga duerme el equipo.
				send_done=1;
				createlogfile(); //crea un nuevo fichero.
			}
		}
	}
	digitalWrite(BEE_sleep,HIGH);
}

//------------------------------------------

void sendData() // envia el contenido de los 5 �ltimos ficheros pendientes de enviar
{
	File myFile;
	char caracter;
	uint8_t payload_1[60]={};
	uint8_t payload_2[]={"error envio de datos$"};
	String fichero_2="";
	char lon_frame[25]={};
	byte line=0;
	SPI.setDataMode(SPI_MODE0);
	digitalWrite(SD_chipSelect, LOW);
	
	//digitalWrite(SD_Sen_powerEnable, HIGH);
	
	for (byte i=0; i<num_file; i++)
	{
		fichero_2 = file_no_send[i];
		if (SD.exists(fichero_2))
		{
			myFile = SD.open(fichero_2, FILE_READ);
			while (myFile.available())
			{
				for (byte i = 0; i < sizeof(data_frame); i++)
				{
					caracter = myFile.read();
					if (caracter == 10)
					{ // c�digo ascii del salto de linea
						break;
					}
					else
					{
						payload_1[i]=(uint8_t) caracter;
					}
				}
				tx_to_coord.setAddress64(addr64_coord);
				tx_to_coord.setPayloadLength(sizeof(payload_1));
				tx_to_coord.setPayload (payload_1);
				xbee.send(tx_to_coord);
				line++;
			}
			fichero_2 ="";
			myFile.close();
			memset (payload_1,0,sizeof(payload_1));
			sprintf(lon_frame, "$lineas_enviadas$%d?", line);
			for (byte i = 0; i < sizeof(lon_frame); i++)
			{
				payload_1[i]=(uint8_t)lon_frame[i];
			}
			tx_to_coord.setAddress64(addr64_coord);
			tx_to_coord.setPayloadLength(sizeof(payload_1));
			tx_to_coord.setPayload(payload_1);
			xbee.send(tx_to_coord);
			line=0;
		}
		else
		{
			tx_to_coord.setAddress64(addr64_coord);
			tx_to_coord.setPayloadLength(sizeof(payload_2));
			tx_to_coord.setPayload (payload_2);
			xbee.send(tx_to_coord);
		}
	}
	digitalWrite(SD_chipSelect, HIGH);
	
	//digitalWrite(SD_Sen_powerEnable, LOW);
	
}

//------------------------------------------

void send_file() // envia el contenido de un fichero que nos piden desde el coordinador
{
	File myFile;
	uint8_t payload_1[60]={};
	uint8_t payload_2[]={"no existe el fichero"};
	String fichero="";
	char lon_frame[25]={};
	byte line=0;
	for (byte i=1; i<=rxResponse.getDataLength();i++)
	{
		if (rxResponse.getData(i)!=58)
		{
			fichero += char (rxResponse.getData(i)) ;
		}
		else
		{
			//fichero += ".txt";
			break;
		}
	}
	SPI.setDataMode(SPI_MODE0);
	digitalWrite(SD_chipSelect, LOW);
	
	//digitalWrite(SD_Sen_powerEnable, HIGH);
	
	if(SD.exists(fichero))
	{
		char caracter;
		myFile = SD.open(fichero, FILE_READ);
		while (myFile.available())
		{
			for (byte i = 0; i < sizeof(data_frame); i++)
			{
				caracter = myFile.read();
				if (caracter == 10)
				{ // c�digo ascii del salto de linea
					break;
				}
				else
				{
					payload_1[i]=(uint8_t) caracter;
				}
			}
			tx_to_coord.setAddress64(addr64_coord);
			tx_to_coord.setPayloadLength(sizeof(payload_1));
			tx_to_coord.setPayload (payload_1);
			xbee.send(tx_to_coord);
			line++;
		}
		myFile.close();
		memset (payload_1,0,sizeof(payload_1));
		sprintf(lon_frame, "$lineas_enviadas$%d?", line);
		for (byte i = 0; i < sizeof(lon_frame); i++)
		{
			payload_1[i]=(uint8_t)lon_frame[i];
		}
		tx_to_coord.setAddress64(addr64_coord);
		tx_to_coord.setPayloadLength(sizeof(payload_1));
		tx_to_coord.setPayload(payload_1);
		xbee.send(tx_to_coord);
		//digitalWrite(SD_chipSelect, HIGH);
	}
	else
	{
		tx_to_coord.setAddress64(addr64_coord);
		tx_to_coord.setPayloadLength(sizeof(payload_2));
		tx_to_coord.setPayload (payload_2);
		xbee.send(tx_to_coord);
	}
	digitalWrite(SD_chipSelect, HIGH);
	
	//digitalWrite(SD_Sen_powerEnable, LOW);
	
}

//------------------------------------------

void send_sincro_rtc() // recibe la hora del coordinador y pone esa hora en el RTC y el micro
{
	String fecha="";
	for (byte i=1; i<=rxResponse.getDataLength();i++)
	{
		if (rxResponse.getData(i)!=46) // codigo ascii con el que terminamos 46 es .
		{
			fecha += char (rxResponse.getData(i)) ;
		}
		else
		{
			break;
		}
	}
	SPI.setDataMode(SPI_MODE1);
	digitalWrite(RTC_chipSelect, LOW);
	t = fecha.toInt();
	setTime(hour(t),minute(t),second(t),day(t),month(t),year(t));
	t_set.sec=second(t);
	t_set.min=minute(t);
	t_set.hour=hour(t);
	t_set.mday=day(t);
	t_set.mon=month(t);
	t_set.year=year(t);
	DS3234_init(RTC_chipSelect, DS3234_INTCN);
	DS3234_set(RTC_chipSelect, t_set);
	digitalWrite(RTC_chipSelect, HIGH);
	if (firstime == true)
	{
		firstime=false;
		createlogfile();
		send_done = 1;
	}
}

//------------------------------------------

void send_cmd_error() // enviamos un error por no haber podido recibir o enviar correctamente los datos
{
	uint8_t payload_1[60]={};
	char lon_frame[25]={};
	sprintf(lon_frame, "$CMD_ERROR$%d?",  int (rxResponse.getData(0)));
	for (byte i = 0; i < sizeof(lon_frame); i++)
	{
		payload_1[i]=(uint8_t)lon_frame[i];
	}
	tx_to_coord.setAddress64(addr64_coord);
	tx_to_coord.setPayloadLength(sizeof(payload_1));
	tx_to_coord.setPayload (payload_1);
	xbee.send(tx_to_coord);
}

//------------------------------------------

void send_ack_endtx() //envia la confirmaci�n de haber enviado los datos correctamente
{
	uint8_t payload_2[60]={};
	char lon_frame[25]={};
	sprintf(lon_frame, "$ack_endtx$0?");
	for (byte i = 0; i < sizeof(lon_frame); i++)
	{
		payload_2[i]=(uint8_t)lon_frame[i];
	}
	tx_to_coord.setAddress64(addr64_coord);
	tx_to_coord.setPayloadLength(sizeof(payload_2));
	tx_to_coord.setPayload (payload_2);
	xbee.send(tx_to_coord);
}

//------------------------------------------

void send_av_data() // envia el numero de ficheros nuevos que tenemos para enviar al coordinador
{
	uint8_t payload_1[60]={};
	char lon_frame[25]={};
	sprintf(lon_frame, "$av_data$%d?", num_file);
	for (byte i = 0; i < sizeof(lon_frame); i++)
	{
		payload_1[i]=(uint8_t)lon_frame[i];
	}
	tx_to_coord.setAddress64(addr64_coord);
	tx_to_coord.setPayloadLength(sizeof(payload_1));
	tx_to_coord.setPayload (payload_1);
	xbee.send(tx_to_coord);
}

//------------------------------------------

void send_time_data() // funcion para cambiar el intervalo de tiempo en que tomamos los datos
{
	String tiempo_toma_datos="";
	for (byte i=1; i<=rxResponse.getDataLength();i++)
	{
		if (rxResponse.getData(i)!=58)
		{
			tiempo_toma_datos += char (rxResponse.getData(i)) ;
		}
		else
		{
			break;
		}
	}
	Intervalo_toma_datos = tiempo_toma_datos.toInt();
}

//------------------------------------------

void createlogfile() // crea un fichero nuevo en la SD
{
	File myFile;
	SPI.setDataMode(SPI_MODE0);
	digitalWrite(SD_chipSelect, LOW);
	
	//digitalWrite(SD_Sen_powerEnable, HIGH);
	
	t=now();
	sprintf(file_name_1,"%d%d%d",day(t),month(t),year(t));
	myFile = SD.open(file_name_1, FILE_WRITE);
	myFile.close();
	if (num_file == 5)
	{
		num_file=0;
		memset (file_no_send,0,sizeof(file_no_send));
	}
	file_no_send[num_file]=(file_name_1);
	num_file++;
	contador_ciclos++;
	digitalWrite(SD_chipSelect, HIGH);
	
	//digitalWrite(SD_Sen_powerEnable, LOW);
	
}

//------------------------------------------

void saveinSD() // graba los datos recogidos en una linea en la SD
{
	File myFile;
	SPI.setDataMode(SPI_MODE0);
	digitalWrite(SD_chipSelect, LOW);
	
	//digitalWrite(SD_Sen_powerEnable, HIGH);
	
	sprintf(data_frame, "$%s$%d.%d;%d.%d;%d.%d;%d.%d;%d;%d$%lu?\r\n", SAN_Endpoint_Id,int(datosTempCorregido_1),abs(int((int(datosTempCorregido_1)-datosTempCorregido_1)*100)),int(datosTempCorregido_2),abs(int((int(datosTempCorregido_2)-datosTempCorregido_2)*100)),int(datosHumedadCorregido_1),abs(int((int(datosHumedadCorregido_1)-datosHumedadCorregido_1)*100)),int(datosHumedadCorregido_2),abs(int((int(datosHumedadCorregido_2)-datosHumedadCorregido_2)*100)),int(bat_level),CHstatus,t_set.unixtime);
	myFile = SD.open(file_name_1, FILE_WRITE);
	if (myFile)
	{
		myFile.print(data_frame);
	}
	myFile.close();
	digitalWrite(SD_chipSelect, HIGH);
	
	//digitalWrite(SD_Sen_powerEnable, LOW);
	
}

//------------------------------------------

void getTH() // toma las medidas de los sensores
{
	
	delay(250);
	
	//digitalWrite(SD_Sen_powerEnable, HIGH);
	
	SHT1x sht1x1(dataPin1, clockPin1);    // Values for Clock and Data pin to the SHT1x class
	SHT1x sht1x2(dataPin2, clockPin2);
	datosHumedadCorregido_1 =0;
	datosHumedadCorregido_1 = sht1x1.readHumidity();
	datosTempCorregido_1 =0;
	datosTempCorregido_1 = sht1x1.readTemperatureC();
	datosHumedadCorregido_2 =0;
	datosHumedadCorregido_2 = sht1x2.readHumidity();
	datosTempCorregido_2 =0;
	datosTempCorregido_2 = sht1x2.readTemperatureC();
	
	//digitalWrite(SD_Sen_powerEnable, LOW);
	
}

//------------------------------------------

void gettimeRTC() // env�a la fecha que tiene el RTC al micro.
{
	SPI.setDataMode(SPI_MODE1);
	digitalWrite(RTC_chipSelect, LOW);
	DS3234_init(RTC_chipSelect, DS3234_INTCN);
	DS3234_get(RTC_chipSelect, &t_set);
	t=t_set.unixtime;
	setTime(hour(t),minute(t),second(t),day(t),month(t),year(t)); //sincronizamos el reloj de arduino con el RTC
	digitalWrite(RTC_chipSelect, HIGH);
}

//------------------------------------------

void readbatlevel() // lee el nivel de bateria
{
	int Batt_counts;
	int Batt_status_counts;
	bat_level=0;
	Batt_counts=analogRead(LIPO_VOL_value);
	//Serial.print("bateria counts:");
	//Serial.println(Batt_counts);
	delayMicroseconds(150);
	bat_level = (Batt_counts*6.45); //6.45 es 6600/1023
	//Serial.print("voltaje bateria:");
	//Serial.println(bat_level);
	Batt_status_counts = analogRead(BATT_status);    // Read the battery voltage, in counts
	CHstatus = read_charger_status(Batt_status_counts); // Read the charger status
}

//------------------------------------------

byte read_charger_status(byte Charger_status_counts) // comprueba el estado de la bateria en cuanto a conexi�n de carga
{
	byte CH_Status=0;
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

//------------------------------------------

void comprobar_RTC() //Comprobamos si es la primera vez que arrancamos el sistema o no
{
	time_t t2;
	SPI.setDataMode(SPI_MODE1);
	digitalWrite(RTC_chipSelect,LOW);
	DS3234_init(RTC_chipSelect, DS3234_INTCN);
	DS3234_get(RTC_chipSelect, &t_set);
	t2=now();
	if (t2 > t_set.unixtime) // si la hora de arduino es mayor que la de rtc el rtc no tiene hora
	{
		hora_rtc = false;
	}
	else
	{
		hora_rtc = true;
	}
	digitalWrite(RTC_chipSelect,HIGH);
}

//------------------------------------------
