#define MQTT_KEEPALIVE 30
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Ajouter flag retain + QOS

#define mqtt_server "192.168.43.3"
#define mqtt_user "sensors"
#define mqtt_password ""
#define window_topic "lghs/sensors/openspace/window/state"
#define door_topic "lghs/sensors/openspace/door/state"
#define probe_name "Wemos_OS"
const char* ssid = "LgHs_IoT";
const char* password = "";

int window_inputPin = D1;
int window_val = 0;

int door_inputPin = D2;
int door_val = 0;

int firstboot = 1;


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(window_inputPin, INPUT_PULLUP);
  pinMode(door_inputPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(window_inputPin), window_interrup, CHANGE);
  attachInterrupt(digitalPinToInterrupt(door_inputPin), door_interrup, CHANGE);
}

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(probe_name, mqtt_user, mqtt_password)) {
      //Serial.println("connected");
      client.publish(window_topic, "Connected");
      break;
    }
    
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 0.2 seconds");
    delay(200);
  }
}
void loop() {
  
  client.loop();
  if (!client.connected()) {
    reconnect();
  }


    window_val = digitalRead(window_inputPin);
    if ((window_val == 1) && (firstboot == 1))
    {
      digitalWrite(LED_BUILTIN, HIGH);
      client.publish(window_topic, "open");
      Serial.println("Window OPEN");
      digitalWrite(LED_BUILTIN, LOW);
    }
    if ((window_val == 0) && (firstboot == 1))
    {
      digitalWrite(LED_BUILTIN, HIGH);
      client.publish(window_topic, "close");
      Serial.println("Window CLOSE");
      digitalWrite(LED_BUILTIN, LOW);
    }
    
    door_val = digitalRead(door_inputPin);
    if ((door_val == 1) && (firstboot == 1))
    {
      digitalWrite(LED_BUILTIN, HIGH);
      client.publish(door_topic, "open");
      Serial.println("Door OPEN");
      digitalWrite(LED_BUILTIN, LOW);
    }
    if ((door_val == 0) && (firstboot == 1))
    {
      digitalWrite(LED_BUILTIN, HIGH);
      client.publish(door_topic, "close");
      Serial.println("Door CLOSE");
      digitalWrite(LED_BUILTIN, LOW);
    }
firstboot = 0;


    delay(500);

}

void window_interrup() {
  if (!client.connected()) {
    reconnect();
  }
  window_val = digitalRead(window_inputPin);
  if ((window_val == 1))
    {
      digitalWrite(LED_BUILTIN, HIGH);
      client.publish(window_topic, "open");
      Serial.println("Window interrupt OPEN");
      digitalWrite(LED_BUILTIN, LOW);
    }
  if ((window_val == 0))
    {
      digitalWrite(LED_BUILTIN, HIGH);
      client.publish(window_topic, "close");
      Serial.println("Window interrupt CLOSE");
      digitalWrite(LED_BUILTIN, LOW);
    }
}
void door_interrup() {
  if (!client.connected()) {
    reconnect();
  }
  door_val = digitalRead(door_inputPin);
  if ((door_val == 1))
    {
      digitalWrite(LED_BUILTIN, HIGH);
      client.publish(door_topic, "open");
      Serial.println("Door interrupt OPEN");
      digitalWrite(LED_BUILTIN, LOW);
    }
  if ((door_val == 0))
    {
      digitalWrite(LED_BUILTIN, HIGH);
      client.publish(door_topic, "close");
      Serial.println("Door interrupt CLOSE");
      digitalWrite(LED_BUILTIN, LOW);
    }
}
