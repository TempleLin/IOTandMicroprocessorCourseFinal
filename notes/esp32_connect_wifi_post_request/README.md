To connect to a WiFi network and send a POST request with the ESP32, you can use the WiFi library and the HTTPClient library. Here is an example of how you can do this:

Include the necessary libraries at the top of your sketch:

    #include <WiFi.h>
    #include <HTTPClient.h>
    
Connect to the WiFi network by creating an instance of the WiFi class and calling its begin function:

    WiFi.begin("your-ssid", "your-password");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");

Create an instance of the HTTPClient class and use its begin function to specify the URL of the server you want to send the request to:

    HTTPClient http;
    http.begin("http://your-server.com/api/post");

Set the request method to "POST" and add any headers or parameters that you want to include in the request:

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Authorization", "Basic YWRtaW46YWRtaW4="); // optional
    http.addParameter("param1", "value1");
    http.addParameter("param2", "value2");

Send the request and check the response:

    int httpCode = http.send();

    if (httpCode > 0) {
        String response = http.getString();
        Serial.println(httpCode);
        Serial.println(response);
    } else {
        Serial.println("Error sending request");
    }

    http.end();

This should send a POST request to the specified URL with the specified headers and parameters, and print the response code and response body to the serial console.

You can find more information about these libraries and how to use them in the Arduino documentation:

WiFi library: https://www.arduino.cc/en/Reference/WiFi
HTTPClient library: https://www.arduino.cc/en/Reference/HTTPClient