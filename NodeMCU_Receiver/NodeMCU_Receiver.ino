#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define MAX 100
#define CAPACITY 300
char bufForH[MAX]; 
char bufForT[MAX];
char bufForHI[MAX]; 

const char* ssid = "Airtel-E5573-E840";
const char* password = "q7i93dbe";

WiFiClient espClient;
PubSubClient client(espClient);

//const char* mqtt_server = "tcp://192.168.1.100:1883";            // RPi Broker IP
const char* mqtt_server = "192.168.1.100";            // RPi Broker IP

void setup_wifi() {

  delay(10);
  // Start by connecting to a WiFi network
  Serial.print("\nConnecting to ");   Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("farmSensor1", "hello humidity");
      client.publish("farmSensor2", "hello temperature");  
      client.publish("farmSensor3", "hello heat index"); 
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("Failed to connect, rc=");
      Serial.print(client.state());
      Serial.println("Trying again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

unsigned int temp = 255;
struct sensor_struct
{
   int moisture;
   int temprature;
   int humidity;
};

struct sensor_struct sensorRecord={ 255 , 255 , 255 };

void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //dht.begin();
}

void loop() {
    delay(2000);
    if (!client.connected()) {
      reconnect();
    }
    client.loop();

    if (Serial.available()>=1)
    {
        //temp = Serial.read();
        //Serial.print(temp);
        //Serial.print("\n");
        //commandByte = Serial.read(); // read 1st byte, if it matches the identifier, continue on
      if (0xf1 == Serial.read())
      {
        sensorRecord.moisture = Serial.read(); // read 2nd byte
        temp=Serial.read();
        sensorRecord.temprature = Serial.read(); // read 4th byte
        temp=Serial.read();
        sensorRecord.humidity = Serial.read(); // read 6th byte
      }
    }
    if (isnan(sensorRecord.moisture) || isnan(sensorRecord.temprature) || isnan(sensorRecord.humidity)) {
      Serial.println(F("Failed to read from sensors!"));
      return;
    }    
    Serial.print("Moisture: ");
    Serial.print(sensorRecord.moisture);
    Serial.print("\n");
    Serial.print("Temprature: ");
    Serial.print(sensorRecord.temprature);
    Serial.print("\n");
    Serial.print("Humidity: ");
    Serial.print(sensorRecord.humidity);
    Serial.print("\n");

    StaticJsonDocument<CAPACITY> doc;
    JsonArray sensorArray = doc.to<JsonArray>();
    
    for(int i=0; i<3; i++) {
      StaticJsonDocument<CAPACITY> doc2;
      JsonObject sensor = doc2.to<JsonObject>();
      sensor["edgeId"] = "e1";
      sensor["sensorId"] = "moisture1";       // Use case
      sensor["sensorType"] = "0";
      switch(i) {
        case 0:
          sensor["sensorValue"] = sensorRecord.moisture;
          break;
        case 1:
          sensor["sensorValue"] = sensorRecord.temprature;
          break;
        case 2:
          sensor["sensorValue"] = sensorRecord.humidity;
          break;
      }
      sensorArray.add(sensor);
    }
    serializeJson(doc, Serial);    
/*  int lenghtSimple = JSONencoder.measureLength();
    Serial.print("Less overhead JSON message size: ");  Serial.println(lenghtSimple);

    Serial.println("Less overhead JSON message: ");
    char JSONmessageBuffer[100];
    
    JSONencoder.printTo(JSONmessageBuffer);
    JSONencoder.printTo(Serial);
*/
    //MQTT Topic is Kafka_Topic 
//    client.publish("Kafka_Topic",doc.as<String>());
//    client.publish("Kafka_Topic",doc.as<Character>());
    char buffer[512];
    size_t n = serializeJson(doc, buffer);
    client.publish("Kafka_Topic2", buffer, n);
    
    Serial.println("\n");
    delay(2000);
    Serial.println("—————");
          
    delay(200);
}
