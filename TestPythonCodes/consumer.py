from flask import Flask
from kafka import KafkaConsumer
import mysql.connector
from json import loads

#app = Flask(__name__)

#localhost:9092 to change to url where kafka runs on
consumer = KafkaConsumer(
    'Kafka_Topic',
     bootstrap_servers=['192.168.1.101:9092'],
     auto_offset_reset='earliest',
     enable_auto_commit=True,
     value_deserializer=lambda x: loads(x.decode('utf-8')))

print("t1")

#mydb = mysql.connector.connect(
#  host="localhost",
#  user="root",
#  passwd=""
#)

#cursor = mydb.cursor()
#cursor.execute("use insightdb");
#cursor.execute("select * from lens_master limit 10;");
#list_rows = cursor.fetchall();

#mydb.close()
# collection = mydb.insightdb.kafkaKube
#consumer = None
while(consumer is not None):
	print("t2")
	for message in consumer:
		#collection.insert_one(message)
		#print('{} added to {}'.format(message, collection))
		#print('{}'.format(message))
		print(message)
print("t3")
#if __name__ == "__main__":
#    app.run()