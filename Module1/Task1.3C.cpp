const int PIR_LED = 13; // Define the pin for the PIR LED
const int TILT_LED = 8; // Define the pin for the TILT LED
const int PIR = 2; // Define the pin for the PIR sensor 
const int TILT = 3; // Define the pin for the TILT sensor

void setup() {
  pinMode(TILT, INPUT_PULLUP); // Set the TILT pin as an input with internal pull-up resistor
  pinMode(PIR, INPUT_PULLUP); // Set the PIR pin as an input with internal pull-up resistor
  pinMode(PIR_LED, OUTPUT); // Set the PIR LED pin as an output  
  pinMode(TILT_LED, OUTPUT); // Set the TILT LED pin as an output
  attachInterrupt(digitalPinToInterrupt(PIR), motionDetected, CHANGE); // Attach interrupt for PIR sensor pin
  attachInterrupt(digitalPinToInterrupt(TILT), tiltDetected, CHANGE); // Attach interrupt for TILT sensor pin
  Serial.begin(9600);
}

void loop() {
  // Empty
}

// Interrupt Service Routine (ISR) called when motion is detected by the PIR sensor
void motionDetected() {
  digitalWrite(PIR_LED, digitalRead(PIR)); // Set the LED based on PIR sensor state
  Serial.println(digitalRead(PIR) ? "Motion detected! LED ON." : "No motion detected! LED OFF.");
}

// Interrupt Service Routine (ISR) called when tilt is detected by the Tilt sensor
void tiltDetected() {
  digitalWrite(TILT_LED, !digitalRead(TILT)); // Set the LED based on TILT sensor state
  Serial.println(!digitalRead(TILT) ? "Tilt detected! TILT LED ON." : "No tilt detected! TILT LED OFF.");
}
