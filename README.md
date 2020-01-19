# HackathonMosaicKisanMitra
Field Sensors >> ArduinoUNO >> (Serial Communication) >> NodeMCU >> (MQTT over Wifi module ESP8266) >> Raspberry Pi >> (KAFKA using mqtt-kafka-bridge Springboot application over Internet) >> AWS Linux Instance >> (To MySQL and Analyze using kafka-mysql-bridge Python application) >> Firebase >> Android app notifications


NOTES Before starting the project :

**1. Publishing data from NodeMCU to RPi broker**
  
For sending the data over MQTT using PubSubClient.h library, the maximum message size, including header, is 128 bytes by default. 

This is configurable via MQTT_MAX_PACKET_SIZE in PubSubClient.h. Check if longer messages can also be sent with the publish_P() method.

Reference : https://github.com/knolleary/pubsubclient#limitations
