#define BLYNK_TEMPLATE_ID "TMPL4Z44hevtV"
#define BLYNK_TEMPLATE_NAME "Alarm Notification"
#define BLYNK_AUTH_TOKEN "HBYMfoEg0MiHSfpN1xDlRrUT0T67pop6"

#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "HBYMfoEg0MiHSfpN1xDlRrUT0T67pop6";
char ssid[] = "John";
char pass[] = "ionutu24";
const int sensorPin = 2;
const int buzzerPin = 5; // Modificați acest pin în funcție de conexiunea buzzer-ului
const int buzzerFrequency = 400; // Modificați frecvența în hertzi

TaskHandle_t buzzerTask;

void activateBuzzer(void *pvParameters);

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  while (Blynk.connected() == false) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Conectat la Blynk!");
  pinMode(sensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // Creează un task pentru buzzer
  xTaskCreate(activateBuzzer, "buzzerTask", 10000, NULL, 1, &buzzerTask);
}

void loop() {
  int motionState = digitalRead(sensorPin);

  if (motionState == HIGH) {
    Blynk.logEvent("m_alarm");
    // Task-ul de buzzer va gestiona sunetul de alarmă
  } else {
    Serial.println("Nicio mișcare.");
  }

  delay(1000); // Așteaptă o secundă între citiri
}

void activateBuzzer(void *pvParameters) {
  while (1) {
    int motionState = digitalRead(sensorPin);

    if (motionState == HIGH) {
      tone(buzzerPin, buzzerFrequency);
      delay(1000); // Sunetul de alarmă durează 1 secundă
      noTone(buzzerPin);
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS); // Așteaptă 1 secundă între verificări
  }
}
