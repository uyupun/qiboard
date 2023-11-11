#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/square_no_alias_2048_int8.h>

const int C_KEY_PIN = 0;
const int D_KEY_PIN = 1;
const int E_KEY_PIN = 2;
const int F_KEY_PIN = 3;
const int G_KEY_PIN = 4;
const int A_KEY_PIN = 5;
const int B_KEY_PIN = 6;
const int CC_KEY_PIN = 7;

const int SHARP_PIN = 2;

const int OCTAVE_DOWN_PIN = 3;
const int OCTAVE_UP_PIN = 4;

const int SPEAKER_PIN = 9;

const int C_NOTE[] = {131, 262, 523};
const int D_NOTE[] = {147, 294, 587};
const int E_NOTE[] = {165, 330, 659};
const int F_NOTE[] = {175, 349, 698};
const int G_NOTE[] = {196, 392, 784};
const int A_NOTE[] = {220, 440, 880};
const int B_NOTE[] = {247, 494, 988};
const int CC_NOTE[] = {262, 523, 1047};

Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> cOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> dOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> eOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> fOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> gOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> aOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> bOscil(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> ccOscil(SQUARE_NO_ALIAS_2048_DATA);

unsigned int amount_threshold = 700;

unsigned int current_octave = 4;
unsigned long octave_up_last_debounce_time = 0;
unsigned long octave_down_last_debounce_time = 0;
unsigned long debounce_delay = 1000;

void setup() {
  Serial.begin(115200);

  pinMode(SHARP_PIN, INPUT_PULLUP);

  pinMode(OCTAVE_DOWN_PIN, INPUT_PULLUP);
  pinMode(OCTAVE_UP_PIN, INPUT_PULLUP);

  pinMode(SPEAKER_PIN, OUTPUT);

  startMozzi();
}

template <unsigned int TABLE_SIZE, unsigned int UPDATE_RATE>
void setFreq(const int note[], Oscil<TABLE_SIZE, UPDATE_RATE>& oscil, int amount, bool is_sharp = false) {
  if (amount > amount_threshold) {
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
  int c_amount = mozziAnalogRead(C_KEY_PIN);
  int d_amount = mozziAnalogRead(D_KEY_PIN);
  int e_amount = mozziAnalogRead(E_KEY_PIN);
  int f_amount = mozziAnalogRead(F_KEY_PIN);
  int g_amount = mozziAnalogRead(G_KEY_PIN);
  int a_amount = mozziAnalogRead(A_KEY_PIN);
  int b_amount = mozziAnalogRead(B_KEY_PIN);
  int cc_amount = mozziAnalogRead(CC_KEY_PIN);

  bool is_sharp = !digitalRead(SHARP_PIN);

  setFreq(C_NOTE, cOscil, c_amount, is_sharp);
  setFreq(D_NOTE, dOscil, d_amount, is_sharp);
  setFreq(E_NOTE, eOscil, e_amount);
  setFreq(F_NOTE, fOscil, f_amount, is_sharp);
  setFreq(G_NOTE, gOscil, g_amount, is_sharp);
  setFreq(A_NOTE, aOscil, a_amount, is_sharp);
  setFreq(B_NOTE, bOscil, b_amount);
  setFreq(CC_NOTE, ccOscil, cc_amount, is_sharp);

  bool is_octave_down = !digitalRead(OCTAVE_DOWN_PIN);
  bool is_octave_up = !digitalRead(OCTAVE_UP_PIN);
  
  downOctave(is_octave_down);
  upOctave(is_octave_up);
}

template <unsigned int TABLE_SIZE, unsigned int UPDATE_RATE>
int makeSound(Oscil<TABLE_SIZE, UPDATE_RATE>& oscil, int amount) {
  if (amount > amount_threshold) {
    return oscil.next();
  }
  return 0;
}

int updateAudio() {
  int c_amount = mozziAnalogRead(C_KEY_PIN);
  int d_amount = mozziAnalogRead(D_KEY_PIN);
  int e_amount = mozziAnalogRead(E_KEY_PIN);
  int f_amount = mozziAnalogRead(F_KEY_PIN);
  int g_amount = mozziAnalogRead(G_KEY_PIN);
  int a_amount = mozziAnalogRead(A_KEY_PIN);
  int b_amount = mozziAnalogRead(B_KEY_PIN);
  int cc_amount = mozziAnalogRead(CC_KEY_PIN);

  int c_sound = makeSound(cOscil, c_amount);
  int d_sound = makeSound(dOscil, d_amount);
  int e_sound = makeSound(eOscil, e_amount);
  int f_sound = makeSound(fOscil, f_amount);
  int g_sound = makeSound(gOscil, g_amount);
  int a_sound = makeSound(aOscil, a_amount);
  int b_sound = makeSound(bOscil, b_amount);
  int cc_sound = makeSound(ccOscil, cc_amount);

  return (c_sound + d_sound + e_sound + f_sound + g_sound + a_sound + b_sound + cc_sound) / 3;
}

void loop() {
  audioHook();
}
