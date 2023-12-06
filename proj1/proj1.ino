#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

const char* ssid     = "John";
const char* password = "ionutu24";

int btnGPIO = 0;
int btnState = false;

void connectToWiFi(void *parameter) {
    Serial.println();
    Serial.print("[WiFi] Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (true) {
        switch(WiFi.status()) {
            case WL_CONNECTED:
                Serial.println("[WiFi] WiFi is connected!");
                Serial.print("[WiFi] IP address: ");
                Serial.println(WiFi.localIP());
                vTaskDelete(NULL);
                break;
            case WL_NO_SSID_AVAIL:
                Serial.println("[WiFi] SSID not found");
                break;
            case WL_CONNECT_FAILED:
                Serial.print("[WiFi] Failed - WiFi not connected! Reason: ");
                break;
            case WL_CONNECTION_LOST:
                Serial.println("[WiFi] Connection was lost");
                break;
            case WL_SCAN_COMPLETED:
                Serial.println("[WiFi] Scan is completed");
                break;
            case WL_DISCONNECTED:
                Serial.println("[WiFi] WiFi is disconnected");
                break;
            default:
                Serial.print("[WiFi] WiFi Status: ");
                Serial.println(WiFi.status());
                break;
        }
        vTaskDelay(pdMS_TO_TICKS(500)); // Pauza de 500 de milisecunde
    }
}

void setup() {
    Serial.begin(115200);
    delay(10);

    // Set GPIO0 Boot button as input
    pinMode(btnGPIO, INPUT);

    xTaskCreatePinnedToCore(
        connectToWiFi,    // Funcția task-ului
        "connectToWiFi",  // Numele task-ului
        8192,             // Stiva task-ului (dimensiunea în octeți)
        NULL,             // Parametrul task-ului (în acest caz, niciunul)
        1,                // Prioritatea task-ului
        NULL,             // Handle-ul task-ului (în acest caz, niciunul)
        1                 // Nucleul procesorului pe care rulează task-ul
    );
}

void loop() {
    // Restul codului poate rămâne neschimbat
    btnState = digitalRead(btnGPIO);
    
    if (btnState == LOW) {
        Serial.println("[WiFi] Disconnecting from WiFi!");
        if(WiFi.disconnect(true, false)){
            Serial.println("[WiFi] Disconnected from WiFi!");
        }
        delay(1000);
    }
}
