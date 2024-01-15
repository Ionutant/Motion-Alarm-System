#define BLYNK_TEMPLATE_ID "TMPL4o7ybbAC3"
#define BLYNK_TEMPLATE_NAME "Motion Alarm"
#define BLYNK_AUTH_TOKEN "hxxF1suN-YimVTyUyMGaRO7RluO7wKfs"

#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "hxxF1suN-YimVTyUyMGaRO7RluO7wKfs";
char ssid[] = "iPhone";
char pass[] = "123456789";
const int sensorPin = 2;
const int buzzerPin = 5; 
const int buzzerFrequency = 800; 

TaskHandle_t buzzerTask;
TaskHandle_t wifiTask;
TaskHandle_t notificationTask;
TaskHandle_t vibrationTask;
SemaphoreHandle_t xMotionSemaphore; 


void activateBuzzer(void *pvParameters);
void connectToWiFi(void *pvParameters);
void readVibrationSensor(void *pvParameters);
void sendNotification(void *pvParameters);

void setup() {
  Serial.begin(115200);
  
 
 
  xTaskCreatePinnedToCore(
        connectToWiFi,
        "wifiTask",
        10000,
        NULL,
        4,
        &wifiTask,
        1
    );

 
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(pdMS_TO_TICKS(1000));
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");

  Blynk.begin(auth, ssid, pass);
  while (Blynk.connected() == false) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Blynk!");

  xMotionSemaphore = xSemaphoreCreateBinary();

  pinMode(sensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

 
  xTaskCreatePinnedToCore(
        activateBuzzer,
        "buzzerTask",
        10000,
        NULL,
        2,
        &buzzerTask,
        1
    );

 
  xTaskCreatePinnedToCore(
        readVibrationSensor,
        "vibrationTask",
        4096,
        NULL,
        3,
        &vibrationTask,
        1
    );


  xTaskCreatePinnedToCore(
        sendNotification,
        "notificationTask",
        8192,
        NULL,
        2,
        &notificationTask,
        1
    );
}

void loop() {

}

void activateBuzzer(void *pvParameters) {
  while (1) {
    int motionState = digitalRead(sensorPin);

    if (motionState == HIGH) {
      tone(buzzerPin, buzzerFrequency);
      delay(1000); 
      noTone(buzzerPin);
    }

    vTaskDelay(100 / portTICK_PERIOD_MS); 
  }
}

void connectToWiFi(void *pvParameters) {
    Serial.println("[WiFi] Connecting to WiFi...");
    
    WiFi.begin(ssid, pass);
    
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }

    Serial.println();
    Serial.println("[WiFi] Connected to WiFi");
     vTaskDelete(NULL);
}


void readVibrationSensor(void *pvParameters) {
  int vibrationlast = 0;
    while (1) {
        
        int sensorValue = digitalRead(sensorPin);
       
     
   if(sensorValue == HIGH && vibrationlast==0){
        xSemaphoreGive(xMotionSemaphore);
   }
   vibrationlast= sensorValue;
        Serial.println("Vibration Sensor Value: " + String(sensorValue));
  //       Serial.print("Valoarea portTICK_PERIOD_MS este: ");
  // Serial.println(portTICK_PERIOD_MS);
        vTaskDelay(pdMS_TO_TICKS(300)); 
    }
}

void sendNotification(void *pvParameters) {
    while (1) {
        if (Blynk.connected()) {
            xSemaphoreTake(xMotionSemaphore, portMAX_DELAY);
           
                Serial.println("Sending Notification...");
                Blynk.logEvent("m_alarm");
               
            vTaskDelay(pdMS_TO_TICKS(100));
        } else {
            Serial.println("Blynk not connected. Reconnecting...");
            Blynk.connect();
            vTaskDelay(pdMS_TO_TICKS(5000)); 
        }
    }
}