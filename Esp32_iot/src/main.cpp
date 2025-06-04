#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Pines
#define LDR_PIN 34       // LDR en ADC1
#define DHT_PIN 15       // Pin de datos del DHT22
#define DHTTYPE DHT22    // Tipo de sensor

// Config WiFi
const char* ssid = "Miguel";
const char* password = "Ruta2005*";

// Config MQTT
const char* mqtt_server = "192.168.245.218";
const int mqtt_port = 1883;
const char* mqtt_user = "";
const char* mqtt_password = "";
const char* mqtt_topic_ldr = "sensor/ldr";
const char* mqtt_topic_temp = "sensor/temperature";
const char* mqtt_topic_hum = "sensor/humidity";

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHT_PIN, DHTTYPE);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(client.state());
      Serial.println(" reintentando en 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // LDR
  int ldr_value = analogRead(LDR_PIN);
  Serial.print("LDR leído: ");
  Serial.println(ldr_value);
  char ldr_str[10];
  snprintf(ldr_str, sizeof(ldr_str), "%d", ldr_value);
  client.publish(mqtt_topic_ldr, ldr_str);

  // DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Temperatura: ");
    Serial.println(temperature);
    Serial.print("Humedad: ");
    Serial.println(humidity);

    char temp_str[10];
    char hum_str[10];
    dtostrf(temperature, 4, 2, temp_str);
    dtostrf(humidity, 4, 2, hum_str);

    client.publish(mqtt_topic_temp, temp_str);
    client.publish(mqtt_topic_hum, hum_str);
  } else {
    Serial.println("Error leyendo el sensor DHT22.");
  }

  delay(2000);
}
