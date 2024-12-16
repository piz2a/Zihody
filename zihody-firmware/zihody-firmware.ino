#include <Adafruit_Keypad.h>
#include <MIDIcontroller.h>

#define ROWS 8
#define COLS 8

byte rowPins[] = {10, 11, 12, 33, 34, 35, 36, 37};
byte colPins[] = {17, 16, 15, 14, 41, 40, 39, 38};
char keys[ROWS][COLS] = {
  {1, 2, 3, 4, 5, 6, 7, 8},
  {9, 10, 11, 12, 13, 14, 15, 16},
  {17, 18, 19, 20, 21, 22, 23, 24},
  {25, 26, 27, 28, 29, 30, 31, 32},
  {33, 34, 35, 36, 37, 38, 39, 40},
  {41, 42, 43, 44, 45, 46, 47, 48},
  {49, 50, 51, 52, 53, 54, 55, 56},
  {57, 58, 59, 60, 61, 62, 63, 64}
};

Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), colPins, rowPins, COLS, ROWS);




byte MIDIchannel = 5;

const int encPinA = 24;   // Change these numbers to
const int encPinB = 25;   // the two pins your encoder is on.

/* Encoder parameters are: 
      1)  pin A (required)
      2)  pin B (required)
      3)  a CC number ( 0 ~ 119 ) 
          OR a channel mode number ( 120 ~ 127 ) 
          OR PROGRAM_CHANGE to send program changes
          (required)
      4)  PER_VALUE or PER_DETENT (optional, PER_DETENT is default) 
 */

MIDIenc myEnc(encPinA, encPinB, 24);
// MIDIenc myEnc(encPinA, encPinB, 24, PER_VALUE); // Faster but not for use with detented encoders.
// MIDIenc myEnc(encPinA, encPinB, PROGRAM_CHANGE); // Send program change instead of CC# 


void setup() {
  Serial.begin(115200);
  customKeypad.begin();
}

void loop() {
  customKeypad.tick();

  while (customKeypad.available()) {
    keypadEvent e = customKeypad.read();
    int key = (int)e.bit.KEY;
    Serial.print(key);
    if (e.bit.EVENT == KEY_JUST_PRESSED) {
      Serial.println(" pressed");
      int channel = 1;
      if (17 <= key && key <= 64) {
        usbMIDI.sendNoteOn(48 - 16 + key, 99, channel);
      }
    } else if (e.bit.EVENT == KEY_JUST_RELEASED) {
      Serial.println(" released");
      int channel = 1;
      if (17 <= key && key <= 64) {
        usbMIDI.sendNoteOff(48 - 16 + key, 0, channel);
      }
    }
  }

  while (usbMIDI.read()) {}

  delay(10);
}
