#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID "imissu@unud.ac.id"
#define WIFI_PASSWORD ""
#define DATABASE_URL "https://wifiscanfirebase-default-rtdb.firebaseio.com/"
#define API_KEY "AIzaSyDRZfCQmKFdjUwuW-lq4P5S3k2wntpP26E" // Ganti dengan Web API Key Anda

FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

unsigned long lastSendTime = 0;
const unsigned long interval = 5000;
bool isOn = false;

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");

    unsigned long startMillis = millis();
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
        if (millis() - startMillis > 10000) {
            Serial.println("\nConnection failed!");
            return;
        }
    }
    
    Serial.println("\nConnected with IP: " + WiFi.localIP().toString());

    // Konfigurasi Firebase
    config.database_url = DATABASE_URL;
    config.api_key = API_KEY;

    // Autentikasi pengguna
    auth.user.email = "surya.rahayuda@gmail.com";
    auth.user.password = "123456";
    Firebase.begin(&config, &auth);
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - lastSendTime >= interval) {
        lastSendTime = currentMillis;

        // Ganti status ON/OFF
        isOn = !isOn;
        String status = isOn ? "ESP8266 On" : "ESP8266 Ready";

        // Buat path unik menggunakan timestamp
        String path = "/device/status/" + String(millis());

        // Kirim data ke Firebase dengan path unik
        if (Firebase.setString(fbdo, path, status)) { 
            Serial.print("Data sent successfully: ");
            Serial.println(status);
        } else {
            Serial.print("Error sending data: ");
            Serial.println(fbdo.errorReason());
        }
    }
}
