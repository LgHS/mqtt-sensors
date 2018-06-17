#define MQTT_KEEPALIVE 30
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN D4
Adafruit_NeoPixel strip = Adafruit_NeoPixel(14, PIN, NEO_GRB + NEO_KHZ800);

#define mqtt_server "192.168.43.3"
#define mqtt_user "sensors"
#define mqtt_password ""
#define sonnette_topic "lghs/sensors/hallway/doorbell"
const char* ssid = "LgHs_IoT";
const char* password = "";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];

void setup() {
  //pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  strip.begin();
  strip.show();
  colorWipe(strip.Color(0, 0, 0), 50);
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
    if (client.connect("Wemos-doorbell", mqtt_user, mqtt_password)) {
      //Serial.println("connected");
      client.publish(sonnette_topic, "Connected");
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

  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);

  if (voltage > 1) {
    Serial.println(voltage);
    Serial.println("On sonne !");
    rainbow(10);
    colorWipe(strip.Color(0, 0, 0), 50);
    if (!client.connected()) {
      reconnect();
    }
    client.publish(sonnette_topic, "DingDong");
    Serial.println("MQTT OK");
    delay(5000);
    Serial.println("Sonnette Prête");
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
