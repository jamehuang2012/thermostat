#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>




#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif



const char* ssid = "CIK1000M_AC2.4G_2863";
const char* password =  "xxxx";
const char* mqttServer = "vps268360.vps.ovh.ca";
const int mqttPort = 1883;
const char* mqttUser = "test";
const char* mqttPassword = "123456";

WiFiClient espClient;
PubSubClient client(espClient);



// define two tasks for Blink & AnalogRead
void TaskMQTT( void *pvParameters );
void TaskCtrl( void *pvParameters );


//Create MQTT Pub message 

/*
JSON format : { curTemp:22.5,status:heat/cool,destTemp:23.5,unit:celsius/fahrenheit}
*/

void createPublicSubMessage() {


  StaticJsonDocument<200> doc;
  doc["curTemp"] = 22.5;
  doc["destTemp"] = 23.5;
  doc["status"] = "Heating";

  serializeJson(doc, Serial);
  serializeJsonPretty(doc, Serial);
}


//MQTT Subscript callback 


void subcriptCallback(char* topic, byte* payload, unsigned int length) {
  
  byte* p = (byte*)malloc(length);
  // Copy the payload to the new buffer
  memcpy(p,payload,length);
  client.publish("test", p, length);
  // Free the memory
  free(p);
  
}

// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);



 //Config WIFI 

  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);

   client.setCallback(subcriptCallback);
 
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

  //client.subscribe("inTest");
  client.publish("test", "Hello from ESP32 App starts");


  

  
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
    TaskMQTT
    ,  "TaskMQTT"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskCtrl
    ,  "AnalogReadA3"
    ,  1024  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
   client.loop();
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

static unsigned long count = 0;
static char   message[30];

void TaskMQTT(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  count = 1;
  // Sending the status to MQTT Broker

  for (;;) // A Task shall never return or exit.
  {
    vTaskDelay(1000);  // one tick delay (15ms) in between reads for stability
    sprintf(message,"From Task TaskMQTT %d",count++);
    client.publish("test", message);
    Serial.println(message);
  }
}

void TaskCtrl(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
/*
  Receive the package from MQTT broker and Parse it
  the data is packed by JSON format
  
*/

  for (;;)
  {
    //Serial.println(sensorValueA3);
    vTaskDelay(1000);  // one tick delay (15ms) in between reads for stability
  }
}
