#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/square_no_alias_2048_int8.h>

const int KEY_C_PIN = 4;
const int KEY_D_PIN = 5;
const int KEY_E_PIN = 6;
const int KEY_F_PIN = 7;
const int KEY_G_PIN = 8;
const int KEY_A_PIN = 10;
const int KEY_B_PIN = 11;
const int KEY_CC_PIN = 12;

const int SPEAKER_PIN = 9;

const int NOTE_C = 261;
const int NOTE_D = 294;
const int NOTE_E = 329;
const int NOTE_F = 349;
const int NOTE_G = 392;
const int NOTE_A = 440;
const int NOTE_B = 493;
const int NOTE_CC = 523;

Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> cOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> dOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> eOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> fOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> gOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> aOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> bOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> ccOscil(SQUARE_NO_ALIAS_2048_DATA);

void setup() {
  pinMode(KEY_C_PIN, INPUT_PULLUP);
  pinMode(KEY_D_PIN, INPUT_PULLUP);
  pinMode(KEY_E_PIN, INPUT_PULLUP);
  pinMode(KEY_F_PIN, INPUT_PULLUP);
  pinMode(KEY_G_PIN, INPUT_PULLUP);
  pinMode(KEY_A_PIN, INPUT_PULLUP);
  pinMode(KEY_B_PIN, INPUT_PULLUP);
  pinMode(KEY_CC_PIN, INPUT_PULLUP);

  pinMode(SPEAKER_PIN, OUTPUT);

  startMozzi();
}

void updateControl() {
  bool key_c = !digitalRead(KEY_C_PIN);
  bool key_d = !digitalRead(KEY_D_PIN);
  bool key_e = !digitalRead(KEY_E_PIN);
  bool key_f = !digitalRead(KEY_F_PIN);
  bool key_g = !digitalRead(KEY_G_PIN);
  bool key_a = !digitalRead(KEY_A_PIN);
  bool key_b = !digitalRead(KEY_B_PIN);
  bool key_cc = !digitalRead(KEY_CC_PIN);

  if (key_c) {
    cOscil.setFreq(NOTE_C);
  }
  else {
    cOscil.setFreq(0);
  }

  if (key_d) {
    dOscil.setFreq(NOTE_D);
  }
  else {
    dOscil.setFreq(0);
  }

  if (key_e) {
    eOscil.setFreq(NOTE_E);
  }
  else {
    eOscil.setFreq(0);
  }

  if (key_f) {
    fOscil.setFreq(NOTE_F);
  }
  else {
    fOscil.setFreq(0);
  }

  if (key_g) {
    gOscil.setFreq(NOTE_G);
  }
  else {
    gOscil.setFreq(0);
  }

  if (key_a) {
    aOscil.setFreq(NOTE_A);
  }
  else {
    aOscil.setFreq(0);
  }

  if (key_b) {
    bOscil.setFreq(NOTE_B);
  }
  else {
    bOscil.setFreq(0);
  }

  if (key_cc) {
    ccOscil.setFreq(NOTE_CC);
  }
  else {
    ccOscil.setFreq(0);
  }
}

int updateAudio() {
  bool key_c = !digitalRead(KEY_C_PIN);
  bool key_d = !digitalRead(KEY_D_PIN);
  bool key_e = !digitalRead(KEY_E_PIN);
  bool key_f = !digitalRead(KEY_F_PIN);
  bool key_g = !digitalRead(KEY_G_PIN);
  bool key_a = !digitalRead(KEY_A_PIN);
  bool key_b = !digitalRead(KEY_B_PIN);
  bool key_cc = !digitalRead(KEY_CC_PIN);

  int c_sound = 0;
  int d_sound = 0;
  int e_sound = 0;
  int f_sound = 0;
  int g_sound = 0;
  int a_sound = 0;
  int b_sound = 0;
  int cc_sound = 0;

  if (key_c) {
    c_sound = cOscil.next();
  }

  if (key_d) {
    d_sound = dOscil.next();
  }

  if (key_e) {
    e_sound = eOscil.next();
  }
  
  if (key_f) {
    f_sound = fOscil.next();
  }

  if (key_g) {
    g_sound = gOscil.next();
  }

  if (key_a) {
    a_sound = aOscil.next();
  }

  if (key_b) {
    b_sound = bOscil.next();
  }

  if (key_cc) {
    cc_sound = ccOscil.next();
  }

  return (c_sound + d_sound + e_sound + f_sound + g_sound + a_sound + b_sound + cc_sound) / 2;
}

void loop() {
  audioHook();
}
