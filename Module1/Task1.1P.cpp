// Define the pin for the LED
int LED_PIN = 13;

// Define the pin for the PIR sensor
int PIR = 2;

void setup()
{
  pinMode(PIR, INPUT); // Set the PIR pin as an input
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as an output
  Serial.begin(9600);
}

void loop()
{
  // Read the digital value from the PIR sensor
  int value = digitalRead(PIR);
  
  Serial.print("PIR Sensor Value: ");
  Serial.println(value);
  
  // Check if the PIR sensor detects motion (HIGH signal)
  if(value == HIGH)
  {
    // Turn on the LED
    digitalWrite(LED_PIN,HIGH);
    Serial.println("Motion detected! LED ON.");
  }
  else
  {
    // Turn off the LED
    digitalWrite(LED_PIN,LOW);
    Serial.println("No motion detected. LED OFF.");
  }
}
