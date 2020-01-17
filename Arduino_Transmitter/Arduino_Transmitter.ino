#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//#include<SoftwareSerial.h> //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
//SoftwareSerial s(3,1);
#include<SoftwareSerial.h>
SoftwareSerial SUART(3, 4); //SRX=Dpin-2; STX-DPin-3
#define DHTPIN 2     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT11     // DHT 11

DHT_Unified dht(DHTPIN, DHTTYPE);


#define moistureAnalogSensorPin  A0
#define LDRAnalogSensorPin  A1

uint32_t delayMS;
int incomingByte = 0; // for incoming serial data

void setup() {
    Serial.begin(9600);
    while (!Serial);// wait for serial port to connect. Needed for native USB port only

    SUART.begin(9600);
    //mySerial.begin(9600);
    //setTX(3);
    //while (!Serial1);// wait for serial port to connect. Needed for native USB port only
    
    dht.begin();
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    delayMS = sensor.min_delay / 1000;
    pinMode(moistureAnalogSensorPin, INPUT);  // for LDR light sensor
    pinMode(LDRAnalogSensorPin, INPUT);  // for Phidget lighet sensor
}

void loop() {

    unsigned int moistureSensorAnalogValue;  
    char moistureSensorDigitalValue;
    unsigned int LDRSensorAnalogValue;
    unsigned int read_buf;
    unsigned int temprature;
    unsigned int humidity;
              //digitalWrite(BUILTIN_LED, LOW);
              //delay(100);
              //digitalWrite(BUILTIN_LED, HIGH);
              //delay(100);
              // put your main code here, to run repeatedly:
            
            
              //if(digitalRead(moistureDigitalSensorPin) == 1)
              //{
               // moistureSensorDigitalValue = 1;
                //digitalWrite(LED_BUILTIN, HIGH);
             // } else 
             // {
                //  moistureSensorDigitalValue = 0;
                 // digitalWrite(LED_BUILTIN, LOW);
              //}
    /************* Moisture sensor ***********/
    //delay(100);
    moistureSensorAnalogValue= analogRead(moistureAnalogSensorPin);
    moistureSensorAnalogValue = map(moistureSensorAnalogValue,0,550,0,100);
    delay(100);
    Serial.print(F("Moisture: "));
    Serial.print(moistureSensorAnalogValue);
    Serial.println(F("%"));      
		/************* Humidity and temperature sensor ***********/
		
		delay(delayMS);
		// Get temperature event and print its value.
		sensors_event_t event;
		dht.temperature().getEvent(&event);
		if (isnan(event.temperature)) {
			Serial.println(F("Error reading temperature!"));
		}
		else {
		Serial.print(F("Temperature: "));
		Serial.print(event.temperature);
    temprature=event.temperature;
		Serial.println(F("°C"));
		}
		// Get humidity event and print its value.
		dht.humidity().getEvent(&event);
		if (isnan(event.relative_humidity)) {
			Serial.println(F("Error reading humidity!"));
		}
		else {
			Serial.print(F("Humidity: "));
			Serial.print(event.relative_humidity);
      humidity=event.relative_humidity;
			Serial.println(F("%"));
		}
		
		/************* Light sensor ***********/
    delay(100);
    Serial.print(F("Light: "));
    Serial.println(LDRSensorAnalogValue);
    //Serial.print(F("\n transmit"));
    
    //SUART.write("19");
    
    //incomingByte=SUART.read();
    //Serial.println(incomingByte);
    SUART.write (0xF1);
    SUART.write((char)moistureSensorAnalogValue);
    SUART.write (0xF2);
    SUART.write((char)temprature);
    SUART.write (0xF3);
    SUART.write((char)humidity);
    //SUART.write('>');                   //>(end mark)
    //SUART.println();
    
}
