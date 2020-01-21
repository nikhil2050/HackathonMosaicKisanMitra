from kafka import KafkaConsumer
import threading
import logging
import time
import json
import mysql.connector
from datetime import datetime
import sys, traceback
from firebase import firebase
from pyfcm import FCMNotification

firebase = firebase.FirebaseApplication('https://hackathonlti.firebaseio.com/', None)

push_service = FCMNotification(api_key="AAAAeob8FZA:APA91bEX2eZoKwgm-2vjeBr712H1d-FngRplKaA_BOZ6LEQtCdIRPYVf3_G2lzGKNMQzy5OGAyOkoHGfjqwex8XgDRFJwMBiPWeoo8NqSlOOCxrh5LHn_fOs7T66teg5FKwkbdH7fx3G")

registration_id ="dk-0vrxeO8U:APA91bERbr9LqcdNUmPMhq4xnjBY1qc7wG-tx0bTj-tRQL2CRgqmwt7vV4lNjCrSNc2xd23ox2liDSKIJVWWMySiuyvty0gNgeIhygF2QYNASTH3NKY9boGurpojWrQCXR5nFJwyHrp6"
message_title = "SensorFluctuation"



mydb = mysql.connector.connect(
    host="localhost",
    user="root",
    passwd="",
	database="insightdb"
)

mycursor = mydb.cursor()
sql = """insert into sensor_table(date_time,hub_id,edge_id,sensor_type,sensor_id,sensor_value) values (%s,%s,%s,%s,%s,%s)"""

class Consumer(threading.Thread):
    daemon = True
    def run(self):
        consumer = KafkaConsumer('Kafka_Topic2',bootstrap_servers='192.168.1.102:9092')
        for message in consumer:
            try:
                my_str = message.value
                hub = message.key.decode("utf-8")
                new_str = my_str.decode("utf-8")
                data = json.loads(new_str)
                payload = json.loads(data["payload"])
                timestamp = int(data["timestamp"])
                timestamp /= 1000
                format = '%Y-%m-%d %H:%M:%S'
                timevalue = datetime.utcfromtimestamp(timestamp).strftime(format)
                for sensor in payload:
                    sensorId = int(sensor["sensorId"])
                    sensorType = int(sensor["sensorType"])
                    sensorValue = int(sensor["sensorValue"])
                    edgeId = sensor["edgeId"]
                    if(sensorType==0):
                       actualType = "moisture"
                    elif(sensorType==1):
                       actualType = "humidity"
                    elif(sensorType==2):
                       actualType = "temperature"
                    else:
                        actualType = "unknown"					
                    if((sensorType==0 and sensorValue>=28) or (sensorType==1 and sensorValue<=55) or (sensorType==2 and sensorValue>=45)):
                        firedata =  {'date_time':timevalue,'hub_id':hub,'edge_id':edgeId,'sensor_type':sensorType,'sensor_id':sensorId,'sensor_value':sensorValue}
                        result = firebase.post('/hackathonlti/CropDB/',firedata)
                        print("Added to FireBase")
                        data_message = {"title": "SensorFluctuation","message":"Please check Sensor value of {} from Sensor Id {} !".format(actualType,sensorId)}
                        push_service.notify_single_device(registration_id=registration_id, message_title=message_title,data_message = data_message)				
                    else:
                        mycursor.execute(sql,(timevalue,hub,edgeId,sensorType,sensorId,sensorValue))
                        print("Added to MySQL")
                        mydb.commit()
            except:
                print("Exception in user code:")
                print('-'*60)
                traceback.print_exc()
                print('-'*60)
        time.sleep(10)

def main():
    threads = [
        Consumer()
    ]
    for t in threads:
        t.start()
    time.sleep(100)
if __name__ == "__main__":
    logging.basicConfig(
        format='%(asctime)s.%(msecs)s:%(name)s:%(thread)d:' +
               '%(levelname)s:%(process)d:%(message)s',
        level=logging.INFO
    )
    main()