#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID "Nethome-Visti"
#define WIFI_PASSWORD "@Bedahuluno14!"
#define DATABASE_URL "https://wifiscanfirebase-default-rtdb.firebaseio.com/"
#define API_KEY "AIzaSyDRZfCQmKFdjUwuW-lq4P5S3k2wntpP26E" // Ganti dengan Web API Key Anda

FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

unsigned long lastSendTime = 0;
const unsigned long interval = 10000; // Interval pengiriman data (10 detik)

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

    // Cek apakah waktu yang telah berlalu lebih besar dari interval
    if (currentMillis - lastSendTime >= interval) {
        lastSendTime = currentMillis;

        // Mulai scan WiFi
        int n = WiFi.scanNetworks();
        Serial.println("Scan completed.");
        
        if (n == 0) {
            Serial.println("No networks found.");
        } else {
            Serial.println("Networks found: " + String(n));
            String path = "/device/wifiScan/" + String(millis()); // Path unik untuk setiap scan
            
            for (int i = 0; i < n; ++i) {
                String ssid = WiFi.SSID(i);
                int rssi = WiFi.RSSI(i);
                
                String networkPath = path + "/network" + String(i);
                String data = "SSID: " + ssid + ", RSSI: " + String(rssi);

                // Kirim data hasil scan ke Firebase
                if (Firebase.setString(fbdo, networkPath, data)) {
                    Serial.println("Data sent successfully: " + data);
                } else {
                    Serial.print("Error sending data: ");
                    Serial.println(fbdo.errorReason());
                }
            }
        }
        
        // Selesai scan, jeda sejenak
        WiFi.scanDelete(); // Bersihkan hasil scan dari memori
    }
}
