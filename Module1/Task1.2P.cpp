// Gaganveer Singh
// 2210994783
// ISR Example

// Define the pin for the LED
int LED_PIN = 13;

// Define the pin for the PIR sensor
int PIR = 2;

void setup()
{
  pinMode(PIR, INPUT_PULLUP); // Set the PIR pin as an input with internal pull-up resistor
  pinMode(LED_PIN, OUTPUT);    // Set the LED pin as an output
  attachInterrupt(digitalPinToInterrupt(PIR), motionDetected, CHANGE); // Attach interrupt for PIR sensor pin
  Serial.begin(9600);
}

void loop()
{
  // Empty
}

// Interrupt Service Routine (ISR) called when motion is detected by the PIR sensor
void motionDetected()
{
  // Check if the PIR sensor detects motion (HIGH signal)
  if(digitalRead(PIR) == HIGH) 
  {
    digitalWrite(LED_PIN, HIGH); // Turn ON the LED
	Serial.println("Motion detected! LED ON.");
  }
  else
  {
    digitalWrite(LED_PIN, LOW); // Turn OFF the LED
    Serial.println("Motion detected! LED OFF.");
  }
}
