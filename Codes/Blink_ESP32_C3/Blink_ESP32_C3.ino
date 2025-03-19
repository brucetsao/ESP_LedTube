
#define BuiltLed 8
#define LedOn LOW
#define LedOff HIGH

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(BuiltLed, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(BuiltLed, LedOn);  // turn the LED on (HIGH is the voltage level)
  delay(8000);                      // wait for a second
  digitalWrite(BuiltLed, LedOff);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
