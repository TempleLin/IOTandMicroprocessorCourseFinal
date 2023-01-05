from mfrc522 import MFRC522
import utime
import time
from machine import UART, Pin

"""
GND - GND
3V3 - 3.3V
GP5 - SDA
GP6 - SDK
GP7 - MOSI
GP4 - MISO
GP22 - RST
"""
reader = MFRC522(spi_id=0, sck=6, miso=4, mosi=7, cs=5, rst=22)
uart0 = UART(0, baudrate=115200, tx=Pin(0), rx=Pin(1))  # Use GP0 and GP1 for UART0.

RFID_BUFFER_CLEAN = 3000  # Refresh last scanned RFID every 3 seconds (3000 milliseconds).

last_scanned_id = ""
last_time_tick = 0


def main():
    global last_scanned_id
    global last_time_tick

    while True:
        current_time = time.ticks_ms()
        if time.ticks_diff(current_time, last_time_tick) > RFID_BUFFER_CLEAN:
            last_scanned_id = ""
            last_time_tick = current_time

        reader.init()
        (stat, tag_type) = reader.request(reader.REQIDL)
        if stat == reader.OK:
            (stat, uid) = reader.SelectTagSN()
            if stat == reader.OK:
                card = int.from_bytes(bytes(uid), "little", False)
                if last_scanned_id == "":
                    print("CARD ID: " + str(card))
                    last_scanned_id = str(card)
                    uart0.write(last_scanned_id)
        utime.sleep_ms(500)


if __name__ == '__main__':
    main()
