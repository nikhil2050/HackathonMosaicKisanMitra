
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

}

void loop() {
    
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
    Serial.print("Moisture: ");
    Serial.print(sensorRecord.moisture);
    Serial.print("\n");
    Serial.print("Temprature: ");
    Serial.print(sensorRecord.temprature);
    Serial.print("\n");
    Serial.print("Humidity: ");
    Serial.print(sensorRecord.humidity);
    Serial.print("\n");
    Serial.print("-----------\n");
    delay(200);
}
