from kafka import KafkaProducer

print("T0")
producer = KafkaProducer(bootstrap_servers='192.168.1.101:9092')
producer.send('Kafka_Topic', b'Hello, World!')
print("T1")
#producer.send('Kafka_Topic', key=b'message-two', value=b'This is Kafka-Python')
print("T3")
