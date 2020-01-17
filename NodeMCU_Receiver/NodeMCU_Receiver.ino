#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define MAX 1000
#define CAPACITY 1000
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
//    String clientId = "ESP8266Client-";
    String clientId = "lens_SBV3NRCHepVEhWcxwczx9cUyTgy";
    
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
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
      Serial.print("MQTT not connected. Reconnecting...");    
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

    String jsonStr = String("[");
    StaticJsonDocument<CAPACITY> doc;
    JsonArray sensorArray = doc.to<JsonArray>();
    for(int i=0; i<3; i++) {
      jsonStr = String(jsonStr + "{\"edgeId\":\"edge1\",\"sensorId\":\"");
      StaticJsonDocument<CAPACITY> doc2;
      JsonObject sensor = doc2.to<JsonObject>();
      sensor["edgeId"] = "edge1";
      switch(i) {
        case 0:
          jsonStr = String(jsonStr + "0\",\"sensorType\":\"0\",\"sensorValue\":\"");
          sensor["sensorId"] = "0";
          sensor["sensorType"] = "0";
          break;
        case 1:
          jsonStr = String(jsonStr + "1\",\"sensorType\":\"1\",\"sensorValue\":\"");
          sensor["sensorId"] = "1";
          sensor["sensorType"] = "1";
          break;
        case 2:
          jsonStr = String(jsonStr + "2\",\"sensorType\":\"2\",\"sensorValue\":\"");
          sensor["sensorId"] = "2";
          sensor["sensorType"] = "2";
          break;
      }
      switch(i) {
        case 0:
          jsonStr = String(jsonStr + sensorRecord.moisture +"\"}");
          sensor["sensorValue"] = sensorRecord.moisture;
          break;
        case 1:
          jsonStr = String(jsonStr + sensorRecord.temprature +"\"}");
          sensor["sensorValue"] = sensorRecord.temprature;
          break;
        case 2:
          jsonStr = String(jsonStr + sensorRecord.humidity +"\"}");
          sensor["sensorValue"] = sensorRecord.humidity;
          break;
      }
      if(i!=2) {
          jsonStr = String(jsonStr + ",");
      }
      sensorArray.add(sensor);
    }
    jsonStr = String(jsonStr + "]");
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
    char buffer[1024];
    size_t n = serializeJson(doc, buffer);
    
//    Serial.print("\njsonStr : ");
//    Serial.println((char*) jsonStr.c_str());
    delay(2000);
//    client.publish("Kafka_Topic2", buffer, 1024);
    client.publish("Kafka_Topic2", (char*) jsonStr.c_str());
    
//    Serial.print(n);
    Serial.println("\nPublished.\n");
    delay(2000);
    Serial.println("—————");
          
    delay(200);
}
