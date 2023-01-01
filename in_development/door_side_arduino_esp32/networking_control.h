#include "WiFiType.h"
#ifndef WIFI_CTRL
#define WIFI_CTRL

// From WIFI library: https://www.arduino.cc/reference/en/libraries/wifi/
#include <WiFi.h>

void printWifiStatus();

/**
  * Make sure Serial.begin() is configured.
  */
void scanNearbyWiFi() {
  // Scan for WiFi networks
  int numNetworks = WiFi.scanNetworks();

  // Print the results to the serial console
  Serial.println("Scan complete");
  Serial.print("Number of networks found: ");
  Serial.println(numNetworks);
  for (int i = 0; i < numNetworks; i++) {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.print(")");
    // Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
  }
  Serial.println("");
}

void connectToWiFi(const char* ssid, const char* password, bool serialEnabled) {
  WiFi.mode(WIFI_STA); // Configure the ESP32 to be Station mode. To prevent it being default as Access Point mode.

  uint8_t status = WL_IDLE_STATUS;

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    if (serialEnabled) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);      
    }
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    WiFi.begin(ssid, password);

    /*
    This is required to make it wait for the correct connection result. The result int returned from WiFi.begin() only gets WL_DISCONNECTED.
    Meaning its connection was not finished.
    */
    status = WiFi.waitForConnectResult();

    if (serialEnabled) {
      Serial.print("Connection result: ");
      Serial.println(status);
    }

    // wait 10 seconds for connection:
    delay(10000);
  }
  if (serialEnabled) {
    Serial.println("Connected to WiFi.");
    printWifiStatus();
  }
}

WiFiClient* connectToServer(const char* server, bool serialEnabled) {
  WiFiClient* client = new WiFiClient();
  if (client->connect(server, 80)) {
    if (serialEnabled) {
      Serial.println("connected to server");
      // Make a HTTP request:
      client->println("GET /search?q=arduino HTTP/1.1");
      client->print("Host: ");
      client->println(server);
      client->println("Connection: close");
      client->println();
    }
  }
  return client;
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

#endif
