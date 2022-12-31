long previousMillis = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    timerCallback();
  }
}

void timerCallback() {
  // This function will be called every 1 second
  Serial.print("One second reached!\n");
}