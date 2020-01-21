import threading
import logging
import time
import json
import mysql.connector
from kafka import KafkaConsumer, KafkaProducer

mydb = mysql.connector.connect(
    host="localhost",
    user="root",
    passwd="",
	database="insightdb"
)

mycursor = mydb.cursor()
sql = "INSERT INTO TempData (test) VALUES ('%s');"
sql2 = "USE insightdb;"

class Producer(threading.Thread):
    daemon = True
    def run(self):
        producer = KafkaProducer(bootstrap_servers='192.168.1.102:9092',
                                 value_serializer=lambda v: json.dumps(v).encode('utf-8'))
        while True:
            producer.send('New_Topic', {"dataObjectID": "test1"})
            producer.send('New_Topic', {"dataObjectID": "test2"})
            print ("Producer Sent")
            time.sleep(1)
			
class Consumer(threading.Thread):
    daemon = True
    def run(self):
        consumer = KafkaConsumer(bootstrap_servers='192.168.1.102:9092',
                                 auto_offset_reset='earliest',
                                 value_deserializer=lambda m: json.loads(m.decode('utf-8')))
        consumer.subscribe(['New_Topic'])
        for message in consumer:
            data = message.value["dataObjectID"]
            mycursor.execute(sql2,vars=None)
            if(mycursor.execute(sql, data)):
                print (data)

def main():
    threads = [
        Producer(),
        Consumer()
    ]
    for t in threads:
        t.start()
    time.sleep(10)
if __name__ == "__main__":
    logging.basicConfig(
        format='%(asctime)s.%(msecs)s:%(name)s:%(thread)d:' +
               '%(levelname)s:%(process)d:%(message)s',
        level=logging.INFO
    )
    main()