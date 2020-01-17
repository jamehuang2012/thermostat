#include <ArduinoJson.h>




#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif



// define two tasks for Blink & AnalogRead
void TaskMQTT( void *pvParameters );
void TaskCtrl( void *pvParameters );


//Create MQTT Pub message 

/*
JSON format : { curTemp:22.5,status:heat/cool,destTemp:23.5,unit:celsius/fahrenheit}
*/


// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);


  Serial.println(ARDUINO_RUNNING_CORE);
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
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskMQTT(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  // Sending the status to MQTT Broker

  for (;;) // A Task shall never return or exit.
  {
    vTaskDelay(1000);  // one tick delay (15ms) in between reads for stability
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
    Serial.println(sensorValueA3);
    vTaskDelay(1000);  // one tick delay (15ms) in between reads for stability
  }
}
