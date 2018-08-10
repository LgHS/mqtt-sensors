#define MQTT_KEEPALIVE 30
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define mqtt_server "192.168.43.3"
#define mqtt_user "sensors"
#define mqtt_password ""
#define window_topic "lghs/sensors/workshop/window/state"
#define probe_name "Wemos_WS"
const char* ssid = "LgHs_IoT";
const char* password = "";

int window_inputPin = D4;
int window_val = 0;


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  //pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(window_inputPin, INPUT);

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
    if ((window_val == 1))
    {
      client.publish(window_topic, "open");
      Serial.println("Window OPEN");
    }
    if ((window_val == 0))
    {
      client.publish(window_topic, "close");
      Serial.println("Window CLOSE");
    }

    delay(200);

}
