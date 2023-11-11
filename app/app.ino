#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/square_no_alias_2048_int8.h>

const int KEY_C_PIN = 0;
const int KEY_D_PIN = 1;
const int KEY_E_PIN = 2;
const int KEY_F_PIN = 3;
const int KEY_G_PIN = 4;
const int KEY_A_PIN = 5;
const int KEY_B_PIN = 6;
const int KEY_CC_PIN = 7;

const int SHARP_PIN = 2;

const int OCTAVE_DOWN_PIN = 3;
const int OCTAVE_UP_PIN = 4;

const int SPEAKER_PIN = 9;

const int NOTE_C[] = {131, 262, 523};
const int NOTE_D[] = {147, 294, 587};
const int NOTE_E[] = {165, 330, 659};
const int NOTE_F[] = {175, 349, 698};
const int NOTE_G[] = {196, 392, 784};
const int NOTE_A[] = {220, 440, 880};
const int NOTE_B[] = {247, 494, 988};
const int NOTE_CC[] = {262, 523, 1047};

Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> cOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> dOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> eOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> fOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> gOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> aOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> bOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> ccOscil(SQUARE_NO_ALIAS_2048_DATA);

unsigned int current_octave = 4;
unsigned long octave_up_last_debounce_time = 0;
unsigned long octave_down_last_debounce_time = 0;
unsigned long debounce_delay = 1000;

void setup() {
  Serial.begin(115200);

  pinMode(SHARP_PIN, INPUT_PULLUP);

  pinMode(OCTAVE_UP_PIN, INPUT_PULLUP);
  pinMode(OCTAVE_DOWN_PIN, INPUT_PULLUP);

  pinMode(SPEAKER_PIN, OUTPUT);

  startMozzi();
}

template <unsigned int TABLE_SIZE, unsigned int UPDATE_RATE>
void setFreq(const int note[], Oscil<TABLE_SIZE, UPDATE_RATE>& oscil, int amount, bool is_sharp = false) {
  if (amount > 700) {
    if (is_sharp) {
      oscil.setFreq((int)(note[current_octave - 3] * 1.059463094359));
    }
    else {
      oscil.setFreq(note[current_octave - 3]);
    }
  }
}

void downOctave(bool is_octave_down) {
  unsigned long current_time = millis();
  if ((current_time - octave_down_last_debounce_time) <= debounce_delay) {
    return;
  }
  if (is_octave_down && current_octave > 3) {
    current_octave--;
    octave_down_last_debounce_time = current_time;
  }
}

void upOctave(bool is_octave_up) {
  unsigned long current_time = millis();
  if ((current_time - octave_up_last_debounce_time) <= debounce_delay) {
    return;
  }
  if (is_octave_up && current_octave < 5) {
    current_octave++;
    octave_up_last_debounce_time = current_time;
  }
}

void updateControl() {
  int c_amount = mozziAnalogRead(KEY_C_PIN);
  int d_amount = mozziAnalogRead(KEY_D_PIN);
  int e_amount = mozziAnalogRead(KEY_E_PIN);
  int f_amount = mozziAnalogRead(KEY_F_PIN);
  int g_amount = mozziAnalogRead(KEY_G_PIN);
  int a_amount = mozziAnalogRead(KEY_A_PIN);
  int b_amount = mozziAnalogRead(KEY_B_PIN);
  int cc_amount = mozziAnalogRead(KEY_CC_PIN);

  bool is_sharp = !digitalRead(SHARP_PIN);

  setFreq(NOTE_C, cOscil, c_amount, is_sharp);
  setFreq(NOTE_D, dOscil, d_amount, is_sharp);
  setFreq(NOTE_E, eOscil, e_amount);
  setFreq(NOTE_F, fOscil, f_amount, is_sharp);
  setFreq(NOTE_G, gOscil, g_amount, is_sharp);
  setFreq(NOTE_A, aOscil, a_amount, is_sharp);
  setFreq(NOTE_B, bOscil, b_amount);
  setFreq(NOTE_CC, ccOscil, cc_amount, is_sharp);

  bool is_octave_down = !digitalRead(OCTAVE_DOWN_PIN);
  bool is_octave_up = !digitalRead(OCTAVE_UP_PIN);
  
  downOctave(is_octave_down);
  upOctave(is_octave_up);
}

template <unsigned int TABLE_SIZE, unsigned int UPDATE_RATE>
int makeSound(Oscil<TABLE_SIZE, UPDATE_RATE>& oscil, int amount) {
  if (amount > 700) {
    return oscil.next();
  }
  return 0;
}

int updateAudio() {
  int c_amount = mozziAnalogRead(KEY_C_PIN);
  int d_amount = mozziAnalogRead(KEY_D_PIN);
  int e_amount = mozziAnalogRead(KEY_E_PIN);
  int f_amount = mozziAnalogRead(KEY_F_PIN);
  int g_amount = mozziAnalogRead(KEY_G_PIN);
  int a_amount = mozziAnalogRead(KEY_A_PIN);
  int b_amount = mozziAnalogRead(KEY_B_PIN);
  int cc_amount = mozziAnalogRead(KEY_CC_PIN);

  int c_sound = 0;
  int d_sound = 0;
  int e_sound = 0;
  int f_sound = 0;
  int g_sound = 0;
  int a_sound = 0;
  int b_sound = 0;
  int cc_sound = 0;

  c_sound = makeSound(cOscil, c_amount);
  d_sound = makeSound(dOscil, d_amount);
  e_sound = makeSound(eOscil, e_amount);
  f_sound = makeSound(fOscil, f_amount);
  g_sound = makeSound(gOscil, g_amount);
  a_sound = makeSound(aOscil, a_amount);
  b_sound = makeSound(bOscil, b_amount);
  cc_sound = makeSound(ccOscil, cc_amount);

  return (c_sound + d_sound + e_sound + f_sound + g_sound + a_sound + b_sound + cc_sound) / 3;
}

void loop() {
  audioHook();
}
