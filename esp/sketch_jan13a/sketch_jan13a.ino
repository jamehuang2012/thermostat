#include <WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "HUAWEI P30";
const char* password =  "Lark2012";
const char* mqttServer = "vps268360.vps.ovh.ca";
const int mqttPort = 1883;
const char* mqttUser = "test";
const char* mqttPassword = "123456";
 
WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
 byte* p = (byte*)malloc(length);
  // Copy the payload to the new buffer
  memcpy(p,payload,length);
  client.publish("test", p, length);
  // Free the memory
  free(p);
}
 
void setup() {
 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);

   client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }

  client.subscribe("inTest");
  client.publish("test", "Hello from ESP32");

  
 
}
 
void loop() {
  client.loop();
}
