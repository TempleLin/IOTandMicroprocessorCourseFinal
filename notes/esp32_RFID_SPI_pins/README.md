The MOSI, MISO, and SCK pins are used to interface with devices over a serial protocol, such as the SPI (Serial Peripheral Interface) protocol.

On the ESP32 microcontroller, the MOSI, MISO, and SCK pins are labeled as follows:

- MOSI (Master Out, Slave In): This is the data output pin for the ESP32. It is used to transmit data from the ESP32 to another device. On the ESP32, the MOSI pin is labeled "IO23".

- MISO (Master In, Slave Out): This is the data input pin for the ESP32. It is used to receive data from another device. On the ESP32, the MISO pin is labeled "IO19".

- SCK (Serial Clock): This is the clock signal pin for the ESP32. It is used to synchronize data transfer between the ESP32 and another device. On the ESP32, the SCK pin is labeled "IO18".

To connect the MOSI, MISO, and SCK pins to a device, you will need to refer to the documentation for the device to determine which pins to use. Some devices may use different pin names or have a different pinout.

For example, to connect the MFRC522 RFID reader/writer module to the ESP32, you would need to connect the MOSI, MISO, and SCK pins of the ESP32 to the corresponding pins on the MFRC522 according to the documentation for the module.