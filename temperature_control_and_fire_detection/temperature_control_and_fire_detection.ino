#include <Buzzer.h>
#include <DHT.h>
#include <Arduino_FreeRTOS.h>
#include "ledconfig.cpp"
#include "mq2config.h"
#include "dht22config.cpp"
#include "buzzerconfig.cpp"
#include "fanconfig.cpp"

void TaskLed5( void *pvParameters );
void TaskLed4( void *pvParameters );
void TaskDht5( void *pvParameters );
void TaskDht4( void *pvParameters );
void TaskMq5( void *pvParameters );
void TaskMq4( void *pvParameters );
void TaskBuzzer5( void *pvParameters );
void TaskBuzzer4( void *pvParameters );
void TaskFan5( void *pvParameters );
void TaskFan4( void *pvParameters );
void TaskPrint( void *pvParameters );

float Ro5;  // you can set the default to 10kohm (10)
float Ro4;  // you can set the default to 10kohm (10)
double temperature5;
double temperature4;
int smoke5;
int smoke4;

void setup() {
  Serial.begin(9600);   //UART setup, baudrate = 9600bps
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }
             
  Serial.println("Calibrating MQ-2 for 5th floor...");                
  Ro5 = MQCalibration(MQ5_PIN);       
  Serial.println("Calibrating MQ-2 for 4th floor...");                  //Calibrating the sensor. Please make sure the sensor is in clean air
  Ro4 = MQCalibration(MQ4_PIN);                       //Calibrating the sensor. Please make sure the sensor is in clean air               
  Serial.println("Calibration is done..."); 
  Serial.println("Ro5 = " + String(Ro5) + " kohm");
  Serial.println("Ro4 = " + String(Ro4) + " kohm");
  dht5.begin();
  dht4.begin();

  xTaskCreate(
    TaskLed5
    ,  "Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
  );

  xTaskCreate(
    TaskLed4
    ,  "Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
  );

  xTaskCreate(
    TaskDht5,
    "GetTemperature1",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskDht4,
    "GetTemperature2",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskMq5,
    "GetSmoke1",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskMq4,
    "GetSmoke2",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskBuzzer5,
    "RingingWhenOnFire",
    128,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskBuzzer4,
    "RingingWhenOnFire",
    128,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskFan5,
    "OnWhenTemperatureIsHot",
    128,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskFan4,
    "OnWhenTemperatureIsHot",
    128,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskPrint,
    "PrintSemuaSensorInputDiTiapLantai",
    128,
    NULL,
    1,
    NULL
  );

  vTaskStartScheduler();
}

void TaskLed5(void *pvParameters){
  pinMode(LED5_PIN, OUTPUT);

  while(1){
    if (smoke5 >= 50){
      digitalWrite(LED5_PIN, HIGH);   
      vTaskDelay( 200 / portTICK_PERIOD_MS ); 
      digitalWrite(LED5_PIN, LOW);    
      vTaskDelay( 200 / portTICK_PERIOD_MS ); 
    }
  }
}

void TaskLed4(void *pvParameters){
  pinMode(LED4_PIN, OUTPUT);

  while(1){
    if (smoke4 >= 50){
      digitalWrite(LED4_PIN, HIGH);   
      vTaskDelay( 200 / portTICK_PERIOD_MS ); 
      digitalWrite(LED4_PIN, LOW);    
      vTaskDelay( 200 / portTICK_PERIOD_MS ); 
    }
  }
}

void TaskDht5(void *pvParameters){

  while(1){
    temperature5 = dht5.readTemperature();
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
  }
}

void TaskDht4(void *pvParameters){

  while(1){
    temperature4 = dht4.readTemperature();
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
  }
}

void TaskMq5(void *pvParameters){
  
  while(1){
    smoke5 = MQGetGasPercentage(MQRead(MQ5_PIN)/Ro5,GAS_SMOKE);
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
  }
}

void TaskMq4(void *pvParameters){
  
  while(1){
    smoke4 = MQGetGasPercentage(MQRead(MQ4_PIN)/Ro4,GAS_SMOKE);
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
  }
}

void TaskBuzzer5(void *pvParameters){

  while(1){
    if (smoke5 >= 50){
      tone(BUZZER5_PIN, 100, 1000);
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
      tone(BUZZER5_PIN, 350, 1000);
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
    }
  }
}

void TaskBuzzer4(void *pvParameters){

  while(1){
    if (smoke4 >= 50){
      tone(BUZZER4_PIN, 100, 1000);
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
      tone(BUZZER4_PIN, 350, 1000);
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
    }
  }
}

void TaskFan5(void *pvParameters){
  pinMode(FAN5_PIN, OUTPUT);

  while(1){
    if (temperature5 >= 35.){
      digitalWrite(FAN5_PIN, HIGH);
    }
    else{
      digitalWrite(FAN5_PIN, LOW);
    }
  }
}

void TaskFan4(void *pvParameters){
  pinMode(FAN4_PIN, OUTPUT);

  while(1){
    if (temperature4 >= 35.){
      digitalWrite(FAN4_PIN, HIGH);
    }
    else{
      digitalWrite(FAN4_PIN, LOW);
    }
  }
}

void TaskPrint(void *pvParameters){
  
  while(1){
    Serial.println(
      String(temperature5) + "," + String(smoke5) + "|" +
      String(temperature4) + "," + String(smoke4) + "|" +
      "<suhu lantai 3>" + "," + "<asap lantai 3>" + "|" +
      "<suhu lantai 2>" + "," + "<asap lantai 2>" + "|" +
      "<suhu lantai 1>" + "," + "<asap lantai 1>" + "|"
    );
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
