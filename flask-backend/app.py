import time
import threading
from flask import Flask, jsonify
import paho.mqtt.client as mqtt
from pymongo import MongoClient

app = Flask(__name__)

mongo_client = MongoClient("mongodb://mongo:27017/")
db = mongo_client["sensor_data"]
ldr_collection = db["ldr"]
dht_collection = db["dht"]

MQTT_BROKER = "mosquitto"
MQTT_PORT = 1883
MQTT_TOPIC_LDR = "sensor/ldr"
MQTT_TOPIC_TEMP = "sensor/temperature"
MQTT_TOPIC_HUM = "sensor/humidity"

def on_connect(client, userdata, flags, rc):
    print("Conectado a MQTT broker con código de resultado: " + str(rc))
    client.subscribe([(MQTT_TOPIC_LDR, 0), (MQTT_TOPIC_TEMP, 0), (MQTT_TOPIC_HUM, 0)])

def on_message(client, userdata, msg):
    payload = msg.payload.decode()
    print(f"Mensaje recibido en {msg.topic}: {payload}")
    if msg.topic == MQTT_TOPIC_LDR:
        try:
            ldr_value = int(payload)
            ldr_collection.insert_one({"value": ldr_value, "timestamp": time.time()})
        except ValueError:
            print(f"Error: no se pudo convertir '{payload}' a int para LDR")
    elif msg.topic == MQTT_TOPIC_TEMP:
        try:
            temp_value = float(payload)
            dht_collection.insert_one({"type": "temperature", "value": temp_value, "timestamp": time.time()})
        except ValueError:
            print(f"Error: no se pudo convertir '{payload}' a float para Temperature")
    elif msg.topic == MQTT_TOPIC_HUM:
        try:
            hum_value = float(payload)
            dht_collection.insert_one({"type": "humidity", "value": hum_value, "timestamp": time.time()})
        except ValueError:
            print(f"Error: no se pudo convertir '{payload}' a float para Humidity")

def mqtt_thread():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.loop_forever()

threading.Thread(target=mqtt_thread, daemon=True).start()

@app.route('/')
def index():
    return jsonify({"message": "Flask backend is running!"})

@app.route('/ldr', methods=['GET'])
def get_ldr():
    data = list(ldr_collection.find({}, {"_id": 0}))
    return jsonify(data)

@app.route('/temperature', methods=['GET'])
def get_temperature():
    data = list(dht_collection.find({"type": "temperature"}, {"_id": 0}))
    return jsonify(data)

@app.route('/humidity', methods=['GET'])
def get_humidity():
    data = list(dht_collection.find({"type": "humidity"}, {"_id": 0}))
    return jsonify(data)

# Run Flask
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
