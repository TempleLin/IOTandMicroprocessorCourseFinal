#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

// Set these to your desired credentials.
const char *ssid = "MeetingIOTCtrl";
const char *password = "meeting1234";

WiFiServer server(80);

enum class EResponse {
  SHOW_DOOR_STATUS,
  REGISTER_USER,
  TEST_POST,
  NONE
};

/*
Attributes that could change when receiving desired requests.
*/
EResponse eResponse = EResponse::SHOW_DOOR_STATUS;

bool doorShouldOpen = false;

String extraToWrite = "";

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

            switch (eResponse) {
              case EResponse::SHOW_DOOR_STATUS:
                client.print(doorShouldOpen? "Open" : "Close");
                break;
              case EResponse::REGISTER_USER:
                client.print("TODO: Implement user registering.");              
                break;
              case EResponse::TEST_POST:
                client.print("Test post request!");
                break;
              case EResponse::NONE:
                client.print("Send some requests!");
                break;
            }

            if (extraToWrite != "") {
              client.print(extraToWrite);
              extraToWrite = "";
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
          eResponse = EResponse::SHOW_DOOR_STATUS;
        } 
        if (currentLine.endsWith("GET /")){
          eResponse = EResponse::NONE;
        }
        if (currentLine.endsWith("POST /test")) {
          eResponse = EResponse::TEST_POST;
        }
        if (currentLine.indexOf("POST /register/") != -1) { // If string contains substring.
          eResponse = EResponse::REGISTER_USER;

          String prefix = "POST /register/";
          int startingIndex = currentLine.indexOf(prefix);
          int endingIndex = currentLine.indexOf("HTTP/1.1"); // POST request might end with "HTTP/1.1".
          
          String subString;
          if (endingIndex != -1) {
            subString = currentLine.substring(startingIndex + prefix.length(), endingIndex);
          } else {
            subString = currentLine.substring(startingIndex + prefix.length()); // Get substring from after prefix index all the way to the end.
          }

          subString.trim();

          extraToWrite = "To register user: " + subString;
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
