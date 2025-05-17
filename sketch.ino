#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";
   // Replace with your Wi-Fi password


const char* apiURL = "http://your.api.url/.'priyanshu"; // Replace with your actual API URL

// Pin definitions
const int ldrPin = 34;                    // LDR sensor connected to analog pin 34
const int fanRelayPin = 0;                // Fan relay connected to GPIO 0
const int lightRelayPin = 2;              // Light relay connected to GPIO 2
const int nightLightRelayPin = 4;         // Night light relay connected to GPIO 4

// Threshold value for light level (adjust based on your room)
const int lightThreshold = 40;

// Variables to store sensor and API values
int ldrValue = 0;
int apiValue = 0;

void setup() {
  Serial.begin(9600);

  // Set relay pins as output
  pinMode(fanRelayPin, OUTPUT);
  pinMode(lightRelayPin, OUTPUT);
  pinMode(nightLightRelayPin, OUTPUT);

  // Set LDR pin as input
  pinMode(ldrPin, INPUT);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
}

void loop() {
  // Read current light level from LDR
  ldrValue = analogRead(ldrPin);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // Fetch control signal from API
  fetchControlFromAPI();

  // Control relays based on logic
  controlNightLight();
  controlFan();
  controlLight();

  delay(1000);  // Delay for 1 second
}

void fetchControlFromAPI() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(apiURL);             // Start connection to API
    int httpCode = http.GET();      // Make GET request

    if (httpCode == 200) {          // 200 OK response
      String response = http.getString();
      apiValue = response.toInt();  // Convert response to integer
      Serial.print("API Value: ");
      Serial.println(apiValue);
    } else {
      Serial.print("Failed to fetch API. HTTP code: ");
      Serial.println(httpCode);
    }

    http.end(); // Close connection
  } else {
    Serial.println("Wi-Fi not connected.");
  }
}

// Controls the night light based on ambient brightness
void controlNightLight() {
  if (ldrValue <= lightThreshold) {
    digitalWrite(nightLightRelayPin, LOW);  // Turn ON night light
  } else {
    digitalWrite(nightLightRelayPin, HIGH); // Turn OFF night light
  }
}

void controlFan() {
  if (apiValue == 2) {
    digitalWrite(fanRelayPin, LOW);   // Turn ON fan
  } else if (apiValue == 1) {
    digitalWrite(fanRelayPin, HIGH);  // Turn OFF fan
  }
  else{
  digitalWrite(fanRelayPin, HIGH);
  }
}

void controlLight() {
  if (apiValue == 3) {
    digitalWrite(lightRelayPin, LOW);   // Turn ON light
  } else if (apiValue == 4) {
    digitalWrite(lightRelayPin, HIGH);  // Turn OFF light
  }
  else {
    digitalWrite(lightRelayPin, HIGH);
  }
}
