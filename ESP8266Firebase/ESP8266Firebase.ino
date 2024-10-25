#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID "imissu@unud.ac.id"
#define WIFI_PASSWORD ""
#define DATABASE_URL "https://wifiscanfirebase-default-rtdb.firebaseio.com/"
#define API_KEY "AIzaSyDRZfCQmKFdjUwuW-lq4P5S3k2wntpP26E" // Ganti dengan Web API Key Anda

FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

unsigned long lastSendTime = 0; // Untuk menyimpan waktu terakhir pengiriman
const unsigned long interval = 5000; // Interval pengiriman data (5 detik)
bool isOn = false; // Status untuk ON/OFF

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
    config.api_key = API_KEY; // Tambahkan API Key Anda

    // Autentikasi pengguna
    auth.user.email = "surya.rahayuda@gmail.com"; // Ganti dengan email pengguna
    auth.user.password = "123456"; // Ganti dengan password pengguna
    Firebase.begin(&config, &auth); // Inisialisasi Firebase dengan autentikasi
}

void loop() {
    unsigned long currentMillis = millis();

    // Cek apakah waktu yang telah berlalu lebih besar dari interval
    if (currentMillis - lastSendTime >= interval) {
        lastSendTime = currentMillis; // Simpan waktu saat ini sebagai waktu terakhir pengiriman

        // Ganti status ON/OFF
        isOn = !isOn;
        String status = isOn ? "ESP8266 On" : "ESP8266 Ready"; // Tentukan status

        // Kirim data ke Firebase
        if (Firebase.setString(fbdo, "/device/status", status)) { 
            Serial.print("Data sent successfully: ");
            Serial.println(status);
        } else {
            Serial.print("Error sending data: ");
            Serial.println(fbdo.errorReason());
        }
    }
}
