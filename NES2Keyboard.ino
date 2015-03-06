/*
 * My version of the NES to USB Arduino code.
 *
 * This is written for the Micro and uses the built-in
 * Keyboard library.
 *
 * Other versions I found depended on the Processing
 * interface, but I wanted to just use the keyboard directly.
 *
 * @Author  Joe Bullard
 */

const int latch = 2;
const int clock = 3;
const int data  = 4;
const int led   = 12;

const int half  = 6;        // Half clock period
const int full  = half * 2; // Full clock period

// NES: Right Left Down Up Start Select B A
// This is using WASD for D-pad
//char keys[] = { 100, 97, 115, 119, KEY_RETURN, 122, KEY_LEFT_CTRL, KEY_LEFT_SHIFT };
char keys[] = { 100, 97, 115, 119, KEY_RETURN, KEY_RIGHT_SHIFT, 122, 120};

// To store button-press bits
byte buttons;

void setup() {
  Serial.begin(9600);
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  Keyboard.begin();
}

void loop() {
  buttons = 0;
  read_NES();
}

/**
 * The NES controller data is read by setting the latch high
 * for 12us, then low for the rest of the cycle.
 * The controller then sends one bit for each button in the
 * order 'A B Select Start Up Down Left Right' each before a
 * clock pulse lasting 12us (i.e. 6us high 6us low), where a
 * 0 indicates that a button is pressed.
 *
 * e.g.  01111111 = A is pressed
 *       10111101 = Both B and Left are pressed
 *       ...
 *
 * The checking takes place outside of the reading loop
 * because I'm afraid it will interfere with the timing.
 */
void read_NES() {
  digitalWrite(latch, LOW);
  digitalWrite(clock, LOW);

  digitalWrite(latch, HIGH);
  delayMicroseconds(full);
  digitalWrite(latch, LOW);
 
  // Read each of the 8 bits
  for (int i = 0; i < 8; i++) {
     buttons = buttons << 1;
     buttons = buttons + digitalRead(data);
 
     // Clock pulse
     digitalWrite(clock, HIGH);
     delayMicroseconds(half);
     digitalWrite(clock, LOW);
     delayMicroseconds(half);
  }
  
  // Check each bit where 0 indicates the pressed state.
  // We have to use press and release so that continuous
  // presses can be registered properly. Otherwise it would
  // reset whatever action is being performed on every read.
  for (int i = 0; i < 8; i++) {
    if ((buttons>>i) % 2 == 0) {
      Keyboard.press(keys[i]);
    } else {
      Keyboard.release(keys[i]);
    }
  }
}
