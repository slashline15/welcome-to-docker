from kafka import KafkaProducer, KafkaConsumer
import json
import time


# Producer
def start_producer():
    producer = KafkaProducer(
        bootstrap_servers=['localhost:9092'],
        value_serializer=lambda v: json.dumps(v).encode('utf-8')
    )

    for i in range(100):
        producer.send('test-topic', {'index': i})
        time.sleep(1)


# Consumer
def start_consumer():
    consumer = KafkaConsumer(
        'test-topic',
        bootstrap_servers=['localhost:9092'],
        auto_offset_reset='earliest',
        enable_auto_commit=True,
        value_deserializer=lambda x: json.loads(x.decode('utf-8'))
    )
    for message in consumer:
        print(f"Consumed message: {message.value}")


start_producer()