#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/square_no_alias_2048_int8.h>

const int KEY_C4_PIN = 2;
const int KEY_E4_PIN = 3;
const int KEY_G4_PIN = 4;

const int SPEAKER_PIN = 9;

const int NOTE_C4 = 261;
const int NOTE_E4 = 329;
const int NOTE_G4 = 392;

Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> oscil(SQUARE_NO_ALIAS_2048_DATA);

void setup() {
  pinMode(KEY_C4_PIN, INPUT_PULLUP);
  pinMode(KEY_E4_PIN, INPUT_PULLUP);
  pinMode(KEY_G4_PIN, INPUT_PULLUP);

  pinMode(SPEAKER_PIN, OUTPUT);

  startMozzi();
}

void updateControl() {
  bool key_c4 = !digitalRead(KEY_C4_PIN);
  bool key_e4 = !digitalRead(KEY_E4_PIN);
  bool key_g4 = !digitalRead(KEY_G4_PIN);

  if (key_c4) {
    oscil.setFreq(NOTE_C4);
  }
  else if (key_e4) {
    oscil.setFreq(NOTE_E4);
  }
  else if (key_g4) {
    oscil.setFreq(NOTE_G4);
  }
  else {
    oscil.setFreq(0);
  }
}

int updateAudio() {
  return oscil.next();
}

void loop() {
  audioHook();
}
