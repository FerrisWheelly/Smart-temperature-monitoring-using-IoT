// ------ Arduino Code----------
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

// -------- WIFI CONFIGURATION --------
const char* ssid = "*****";
const char* password = "**********";

// -------- THINGSPEAK CONFIGURATION --------
unsigned long channelID = ********;
const char* writeAPIKey = "********";
const char* readAPIKey  = "*********";  // Used to read cloud control (manual OFF)

WiFiClient client;

// -------- DHT SENSOR SETUP --------
#define DHTPIN D4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// -------- OUTPUT PINS --------
#define RED_LED D5
#define GREEN_LED D6
#define BUZZER D7

// -------- THRESHOLDS --------
float thresholdTemp = 31.0;       // Temperature limit for alert
float thresholdHumidity = 100.0;  // (Not used currently)

// -------- BUZZER TIMING CONTROL --------
unsigned long buzzerTimer = 0;
bool buzzerState = false;  // Tracks ON/OFF state of buzzer

void setup() {
  Serial.begin(115200);
dht.begin();  // Initialize DHT sensor
// Set output devices
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
// Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
// Initialize ThingSpeak communication
  ThingSpeak.begin(client);
}
void loop() {
// -------- READ SENSOR DATA --------
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
int alert = 0;  // Flag to send to cloud
 Serial.println("----- SENSOR DATA -----");
  Serial.print("Temp: "); Serial.println(temp);
  Serial.print("Humidity: "); Serial.println(hum);

  // -------- READ MANUAL CONTROL FROM CLOUD --------
  // Field 4 is used to remotely turn OFF buzzer
  int manualOff = ThingSpeak.readIntField(channelID, 4, readAPIKey);

  // Check if sensor values are valid
  if (!isnan(temp) && !isnan(hum)) {

    // -------- TEMPERATURE CONDITION --------
    if (temp > thresholdTemp) {
      alert = 1;
      digitalWrite(RED_LED, HIGH);   // High temp → Red LED ON
      digitalWrite(GREEN_LED, LOW);
      Serial.println("⚠ HIGH TEMPERATURE!");
    }
    else {
      alert = 0;
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH); // Normal → Green LED ON
     // Turn OFF buzzer in normal condition
      digitalWrite(BUZZER, LOW);
      buzzerState = false;

      Serial.println("Normal Condition");
    }
// -------- BUZZER CONTROL (WITH TIMER) --------
    // Works only if temp is high AND not manually disabled
    if (temp > thresholdTemp && manualOff == 0) {

      unsigned long currentTime = millis();

      // Buzzer ON/OFF cycling logic
      if (buzzerState) {
        // Keep buzzer ON for 5 seconds
        if (currentTime - buzzerTimer >= 5000) {
          buzzerState = false;
          buzzerTimer = currentTime;
          digitalWrite(BUZZER, LOW);
        }
      } else {
        // Turn buzzer ON every 10 seconds
        if (currentTime - buzzerTimer >= 10000) {
          buzzerState = true;
          buzzerTimer = currentTime;
          digitalWrite(BUZZER, HIGH);
        }
      }
 }
    else {
      // Turn OFF buzzer if temp normal OR manual OFF enabled
      digitalWrite(BUZZER, LOW);
      buzzerState = false;

      if (manualOff == 1)
        Serial.println(" Buzzer OFF from cloud");
    }

    // -------- SEND DATA TO THINGSPEAK --------
    ThingSpeak.setField(1, temp);    // Field 1 → Temperature
    ThingSpeak.setField(2, hum);     // Field 2 → Humidity
    ThingSpeak.setField(3, alert);   // Field 3 → Alert status

    ThingSpeak.writeFields(channelID, writeAPIKey);
  }

  Serial.println("------------------------\n");

  delay(3000);  // Wait before next reading
}
