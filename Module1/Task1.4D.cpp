// Student Name: Gaganveer Singh
// Student RollNo: 2210994783
// SIT315 Module 1 Task 1.4D

/* 
  Calculations (for 500ms):
  System clock 16 Mhz and Prescalar 256;
  Timer1 speed = 16Mhz/256 = 62500Hz
  Count for 500ms = 500ms * 62500Hz = 31250
*/

const int PIR = 2;     // pin for the PIR sensor
const int PIR_LED = 8; // pin for the PIR LED

const int TILT = 3;      // pin for the TILT sensor
const int TILT_LED = 10; // pin for the TILT LED

const int SWTICH = 4;
const int SWTICH_LED = 12;

const int TIMER_LED = 13;
bool LED_STATE = true;

volatile byte motionState, tiltState, switchState;

void setup()
{
  pinMode(TILT, INPUT_PULLUP); // TILT pin as an input
  pinMode(PIR, INPUT);  // PIR pin as an input 
  pinMode(SWTICH, INPUT);

  pinMode(PIR_LED, OUTPUT);    // PIR LED pin as an output
  pinMode(TILT_LED, OUTPUT);   // TILT LED pin as an output
  pinMode(SWTICH_LED, OUTPUT); // switch as input with pullup

  PCICR |= B00000100;  // Using Port D
  PCMSK2 |= B00011100; // Pin D2, D3 and D4

  // Reading intial sensor states
  motionState = digitalRead(PIR);
  tiltState = !digitalRead(TILT);
  switchState = digitalRead(SWTICH);

  cli(); // Stop interrupts for settings
  /*1. Reseting the control register.*/
  TCCR1A = 0; // Reset entire TCCR1A to 0
  TCCR1B = 0; // Reset entire TCCR1B to 0

  /*2. Setting prescaler by changig bits */
  TCCR1B |= B00000100; // CS12 to 1 so prescalar 256

  /*3. Enabling compare match mode on register A*/
  TIMSK1 |= B00000010;
  /*4. Set the value of register A to 31250*/
  OCR1A = 31250; // Finally we set compare register A to this value
  sei();

  Serial.begin(9600);
}

void loop()
{
  printSensorStates();
}

void printSensorStates()
{
  // Print sensor states in rows and columns
  Serial.print("PIR State: ");
  Serial.print(motionState);
  Serial.print("\t");
  Serial.print("TILT State: ");
  Serial.print(tiltState);
  Serial.print("\t");
  Serial.print("Switch State: ");
  Serial.println(switchState);
}

ISR(TIMER1_COMPA_vect)
{
  TCNT1 = 0;                          // Timer back to 0 so it resets for next interrupt
  LED_STATE = !LED_STATE;             // Invert LED state
  digitalWrite(TIMER_LED, LED_STATE); // Update LED state
}

ISR(PCINT2_vect)
{
  // Read the current state of PIR, TILT, and button pins
  motionState = (PIND & B00000100) ? 1 : 0;
  tiltState = (PIND & B00001000) ? 0 : 1; // inverting as sensor works 
  switchState = (PIND & B00010000) ? 1 : 0;

  // Update LED states based on the states
  digitalWrite(PIR_LED, motionState);
  digitalWrite(TILT_LED, tiltState);
  digitalWrite(SWTICH_LED, switchState);
}
