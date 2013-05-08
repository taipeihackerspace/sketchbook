/*
  Doorbell interceptor and ring cycler.
  Detects the ring trigger from the wireless button, changes the ringtone, and sends a ring notification over the serial port.
 */

/* We modified the doorbell to have 5 inputs: select 24, select 17, ground, V+. trigger */
/* connected pin 12 to select, pin 11 to trigger, 5V to V+, and GND to ground */
int select = 12;
int trigger = 11;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize serial communication
  Serial.begin(9600);
  // Arduino will write HIGH to the select, which tells the receiver to change the song
  pinMode(select, OUTPUT);
  // we expect it to be LOW by default
  digitalWrite(select, LOW);
  // Arduino listens for someone pressing the button to trigger the bell
  pinMode(trigger, INPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
  // if someone presses the button, this sets the trigger to HIGH
  if (digitalRead(trigger) == HIGH) {
    // we override the trigger to LOW so we can change the song
    // as long as the trigger is HIGH, we cannot change the song (that's how the receiver/bell works)
    pinMode(trigger, OUTPUT);
    digitalWrite(trigger, LOW);
    // we change the song
    digitalWrite(select, HIGH);
    // we notify over the serial port that the bell has been rung
    Serial.println("ring!!");
    // near the shortest time the select should be set to HIGH to actually change the song
    delay(50);
    // back to the default setup where Arduino listens for the trigger
    digitalWrite(select, LOW);
    pinMode(trigger, INPUT);
    
  }
  // a small delay so we don't loop too fast
  delay(10);
}
