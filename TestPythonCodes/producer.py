from flask import Flask
from time import sleep
from json import dumps
from kafka import KafkaProducer

#app = Flask(__name__)

#localhost:9092 to change to url where kafka runs on
producer = KafkaProducer(bootstrap_servers=['192.168.1.101:9092'],value_serializer=lambda x:dumps(x).encode('utf-8'))

for e in range(1000):
    data = {'number' : e}
    producer.send('numtest', value=data)
    sleep(5)