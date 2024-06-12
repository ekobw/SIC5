#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "";
const char* password = "";

// Replace with your server URL from Python Script
const char* serverName = "";

// DHT Sensor
#define DHTPIN 4 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT11 or DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize the DHT sensor
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Send data to server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/json");

    // Data to send with HTTP POST
    String httpRequestData = "{\"temperature\":" + String(t) + ",\"humidity\":" + String(h) + "}";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    // If you need an HTTP response
    if (httpResponseCode > 0) {
      String response = http.getString(); //Get the response to the request
      Serial.println(httpResponseCode);   //Print return code
      Serial.println(response);           //Print request answer
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
