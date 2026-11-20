#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "etusiemens";
const char* password = "etusiemens";

const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_user = "";
const char* mqtt_pass = "";

WiFiClient espClient;
PubSubClient client(espClient);

String mensaje;
String topico = "";

// Funcion para recibir mensajes del broker MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Topic: ");
  topico += topic;
  Serial.println(topic);
  Serial.print("Contenido: ");

  for (unsigned int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }

  Serial.println(mensaje);
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("Conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("Conectado");
      client.subscribe("casa/led");
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(client.state());
      Serial.println(" - reintentando en 5 s");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}

/*
Ejemplo de envío de un mensaje a un topic (fragmento de la página 7):

String msg = "mensaje a enviar";
client.publish("topic", msg.c_str());

Este fragmento es un ejemplo independiente. Para usarlo, colócalo donde
corresponda en tu programa; no se ejecuta automáticamente en este sketch.
*/
