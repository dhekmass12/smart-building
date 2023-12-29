#include <Buzzer.h>
#include <DHT.h>
#include <Arduino_FreeRTOS.h>
#include "ledconfig.cpp"
#include "mq2config.h"
#include "dht22config.cpp"
#include "buzzerconfig.cpp"
#include "fanconfig.cpp"

void TaskLed1( void *pvParameters );
void TaskLed2( void *pvParameters );
void TaskDht1( void *pvParameters );
void TaskDht2( void *pvParameters );
void TaskMq1( void *pvParameters );
void TaskMq2( void *pvParameters );
void TaskBuzzer1( void *pvParameters );
void TaskBuzzer2( void *pvParameters );
void TaskFan1( void *pvParameters );
void TaskFan2( void *pvParameters );
void TaskPrint( void *pvParameters );

float Ro1;  // you can set the default to 10kohm (10)
float Ro2;  // you can set the default to 10kohm (10)
double temperature1;
double temperature2;
int smoke1;
int smoke2;

void setup() {
  Serial.begin(9600);   //UART setup, baudrate = 9600bps
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }
             
  Serial.println("Calibrating MQ1...");                
  Ro1 = MQCalibration(MQ1_PIN);       
  Serial.println("Calibrating MQ2...");                  //Calibrating the sensor. Please make sure the sensor is in clean air
  Ro2 = MQCalibration(MQ2_PIN);                       //Calibrating the sensor. Please make sure the sensor is in clean air               
  Serial.println("Calibration is done..."); 
  Serial.println("Ro1 = " + String(Ro1) + " kohm");
  Serial.println("Ro2 = " + String(Ro2) + " kohm");
  dht1.begin();
  dht2.begin();

  xTaskCreate(
    TaskLed1
    ,  "Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
  );

  xTaskCreate(
    TaskLed2
    ,  "Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
  );

  xTaskCreate(
    TaskDht1,
    "GetTemperature1",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskDht2,
    "GetTemperature2",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskMq1,
    "GetSmoke1",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskMq2,
    "GetSmoke2",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskBuzzer1,
    "RingingWhenOnFire",
    128,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskBuzzer2,
    "RingingWhenOnFire",
    128,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskFan1,
    "OnWhenTemperatureIsHot",
    128,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskFan2,
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

void TaskLed1(void *pvParameters){
  pinMode(LED1_PIN, OUTPUT);

  while(1){
    if (smoke1 >= 50){
      digitalWrite(LED1_PIN, HIGH);   
      vTaskDelay( 200 / portTICK_PERIOD_MS ); 
      digitalWrite(LED1_PIN, LOW);    
      vTaskDelay( 200 / portTICK_PERIOD_MS ); 
    }
  }
}

void TaskLed2(void *pvParameters){
  pinMode(LED2_PIN, OUTPUT);

  while(1){
    if (smoke2 >= 50){
      digitalWrite(LED2_PIN, HIGH);   
      vTaskDelay( 200 / portTICK_PERIOD_MS ); 
      digitalWrite(LED2_PIN, LOW);    
      vTaskDelay( 200 / portTICK_PERIOD_MS ); 
    }
  }
}

void TaskDht1(void *pvParameters){

  while(1){
    temperature1 = dht1.readTemperature();
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
  }
}

void TaskDht2(void *pvParameters){

  while(1){
    temperature2 = dht2.readTemperature();
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
  }
}

void TaskMq1(void *pvParameters){
  
  while(1){
    smoke1 = MQGetGasPercentage(MQRead(MQ1_PIN)/Ro1,GAS_SMOKE);
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
  }
}

void TaskMq2(void *pvParameters){
  
  while(1){
    smoke2 = MQGetGasPercentage(MQRead(MQ2_PIN)/Ro2,GAS_SMOKE);
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
  }
}

void TaskBuzzer1(void *pvParameters){

  while(1){
    if (smoke1 >= 50){
      tone(BUZZER1_PIN, 100, 1000);
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
      tone(BUZZER1_PIN, 350, 1000);
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
    }
  }
}

void TaskBuzzer2(void *pvParameters){

  while(1){
    if (smoke2 >= 50){
      tone(BUZZER2_PIN, 100, 1000);
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
      tone(BUZZER2_PIN, 350, 1000);
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
    }
  }
}

void TaskFan1(void *pvParameters){
  pinMode(FAN1_PIN, OUTPUT);

  while(1){
    if (temperature1 >= 35.){
      digitalWrite(FAN1_PIN, HIGH);
    }
    else{
      digitalWrite(FAN1_PIN, LOW);
    }
  }
}

void TaskFan2(void *pvParameters){
  pinMode(FAN2_PIN, OUTPUT);

  while(1){
    if (temperature2 >= 35.){
      digitalWrite(FAN2_PIN, HIGH);
    }
    else{
      digitalWrite(FAN2_PIN, LOW);
    }
  }
}

void TaskPrint(void *pvParameters){
  
  while(1){
    Serial.println(
      String(temperature1) + "," + String(smoke1) + "|" +
      String(temperature2) + "," + String(smoke2) + "|" +
      "<suhu lantai 3>" + "," + "<asap lantai 3>" + "|" +
      "<suhu lantai 2>" + "," + "<asap lantai 2>" + "|" +
      "<suhu lantai 1>" + "," + "<asap lantai 1>" + "|" +
    );
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
