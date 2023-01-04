#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

// Set these to your desired credentials.
const char *ssid = "MeetingIOTCtrl";
const char *password = "meeting1234";

WiFiServer server(80);

/*
Attributes that could change when receiving desired requests.
*/
bool showIsDoorOpen = false;
bool doorShouldOpen = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: "); 
  Serial.println(myIP); // http://192.168.4.1/
  server.begin();

  Serial.println("Server started");
}

/*
Researched WiFi server code examples:
- https://randomnerdtutorials.com/esp32-web-server-arduino-ide/
*/

void loop() {
WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            if (showIsDoorOpen) {
              client.print(doorShouldOpen? "Open" : "Close");
            } else {
              client.print("Send some requests!");              
            }

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /canDoorOpen".
        if (currentLine.endsWith("GET /canDoorOpen")) {
          showIsDoorOpen = true;
        } else if (currentLine.endsWith("GET /")){
          showIsDoorOpen = false;
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
