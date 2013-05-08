/*
05-08-2013 
4 digit 7 segment type: LF0565SRWK, Common negative
Details about 4 digit 7 segment:
http://myyp090082.myyp.com.tw/ec99/myyp090082/GoodsDescr.asp?category_id=41&parent_id=0&prod_id=2122
Modified the code a little bit from:
http://www.hobbytronics.co.uk/arduino-4digit-7segment

*/

/*
 6-13-2011
 Spark Fun Electronics 2011
 Nathan Seidle
 
 This code is public domain but you buy me a beer if you use this and we meet 
 someday (Beerware license).
 
 4 digit 7 segment display:
 http://www.sparkfun.com/products/9483
 Datasheet: 
 http://www.sparkfun.com/datasheets/Components/LED/7-Segment/YSD-439AR6B-35.pdf

 This is an example of how to drive a 7 segment LED display from an ATmega
 without the use of current limiting resistors. This technique is very common 
 but requires some knowledge of electronics - you do run the risk of dumping 
 too much current through the segments and burning out parts of the display. 
 If you use the stock code you should be ok, but be careful editing the 
 brightness values.
 
 This code should work with all colors (red, blue, yellow, green) but the 
 brightness will vary from one color to the next because the forward voltage 
 drop of each color is different. This code was written and calibrated for the 
 red color.

 This code will work with most Arduinos but you may want to re-route some of 
 the pins.

 7 segments
 4 digits
 1 colon
 =
 12 pins required for full control 
 
 */

int digit1 = 11; //PWM Display pin 1
int digit2 = 10; //PWM Display pin 2
int digit3 = 9; //PWM Display pin 6
int digit4 = 6; //PWM Display pin 8

//Pin mapping from Arduino to the ATmega DIP28 if you need it
//http://www.arduino.cc/en/Hacking/PinMapping
int segA = A1; //Display pin 14
int segB = 3; //Display pin 16
int segC = 4; //Display pin 13
int segD = 5; //Display pin 3
int segE = A0; //Display pin 5
int segF = 7; //Display pin 11
int segG = 8; //Display pin 15

void setup() {                
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
  
  pinMode(13, OUTPUT);
}

void loop() {
  
  //long startTime = millis();

  displayNumber(millis()/100);
  /*
  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, HIGH);
  digitalWrite(digit3, HIGH);
  digitalWrite(digit4, HIGH);
  
  digitalWrite(digit4, LOW);
  
  lightNumber(7);
  */
  //while( (millis() - startTime) < 2000) {
  //displayNumber(1217);
  //}
  //delay(1000);  
}

//Given a number, we display 10:22
//After running through the 4 numbers, the display is left turned off

//Display brightness
//Each digit is on for a certain amount of microseconds
//Then it is off until we have reached a total of 20ms for the function call
//Let's assume each digit is on for 1000us
//Each digit is on for 1ms, there are 4 digits, so the display is off for 16ms.
//That's a ratio of 1ms to 16ms or 6.25% on time (PWM).
//Let's define a variable called brightness that varies from:
//5000 blindingly bright (15.7mA current draw per digit)
//2000 shockingly bright (11.4mA current draw per digit)
//1000 pretty bright (5.9mA)
//500 normal (3mA)
//200 dim but readable (1.4mA)
//50 dim but readable (0.56mA)
//5 dim but readable (0.31mA)
//1 dim but readable in dark (0.28mA)

void displayNumber(int toDisplay) {
#define DISPLAY_BRIGHTNESS  500

#define DIGIT_ON  LOW
#define DIGIT_OFF  HIGH

#define NUM_LED_DIGITS 4 // this is the physical number of 7-segment LEDs

#define NUM_DIGITS 2 // this is the number of digits we want to display before resetting
                     // should be a number between 1 and NUM_LED_DIGITS

  long beginTime = millis();
  
  int digits[5];
  int toDisplayOriginal = toDisplay;
  
  // identify digits
  for (int digit = NUM_DIGITS ; digit > 0 ; digit--) {
    digits[digit] = toDisplay % 10;
    toDisplay /= 10;
  }
  
  // don't display initial 0 values by checking digits, starting with the most significant one
  for (int digit = 1 ; digit <= NUM_DIGITS ; digit++) {
    if (digits[digit] == 0) {
      digits[digit] = 10; // an initial 0 should not be displayed, so replace 0 with "don't display" value
    } else {
      break; // we have a non-zero value, so this digit and any further digits should be displayed, so we stop checking
    }
  }

  for (int digit = NUM_DIGITS ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit + (NUM_LED_DIGITS - NUM_DIGITS)) {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      break;
    case 2:
      digitalWrite(digit2, DIGIT_ON);
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
      break;
    }

    //Turn on the right segments for this digit
    lightNumber(digits[digit]);

    delayMicroseconds(DISPLAY_BRIGHTNESS); 
    //Display digit for fraction of a second (1us to 5000us, 500 is pretty good)

    //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
  }
  
  // add a little animation if one of the LEDs is not used
  
#define ANIMATION_SLOWNESS 2

  if (NUM_DIGITS < NUM_LED_DIGITS) {
    // use first, unused LED for something else
    digitalWrite(digit1, DIGIT_ON);
    lightSegment(toDisplayOriginal/ANIMATION_SLOWNESS % 14);
    delayMicroseconds(DISPLAY_BRIGHTNESS);
    digitalWrite(digit1, DIGIT_OFF);
  }

  
  while( (millis() - beginTime) < 10) ; 
  //Wait for 20ms to pass before we paint the display again
}

#define SEGMENT_ON  HIGH//LOW
#define SEGMENT_OFF LOW//HIGH

//Given a number, turns on those segments
//If number == 10, then turn off number
void lightNumber(int numberToDisplay) {

  switch (numberToDisplay){

  case 0:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 1:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 2:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 3:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 4:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 5:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 6:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 7:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 8:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 9:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 10:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;
  }
}

//Given a number, turns on those segments
//If number == 10, then turn off number
void lightSegment(int segmentToDisplay) {

  switch (segmentToDisplay){

  case 0:
    digitalWrite(segA, SEGMENT_ON);
    break;

  case 1:
    digitalWrite(segB, SEGMENT_ON);
    break;

  case 2:
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 3:
    digitalWrite(segE, SEGMENT_ON);
    break;

  case 4:
    digitalWrite(segD, SEGMENT_ON);
    break;

  case 5:
    digitalWrite(segC, SEGMENT_ON);
    break;

  case 6:
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 7:
    digitalWrite(segF, SEGMENT_ON);
    break;

  case 8:
    digitalWrite(segA, SEGMENT_ON);
    break;

  case 9:
    digitalWrite(segB, SEGMENT_ON);
    break;

  case 10:
    digitalWrite(segC, SEGMENT_ON);
    break;

  case 11:
    digitalWrite(segD, SEGMENT_ON);
    break;

  case 12:
    digitalWrite(segE, SEGMENT_ON);
    break;

  case 13:
    digitalWrite(segF, SEGMENT_ON);
    break;

  }
}
