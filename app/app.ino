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

int current_octave = 4;
unsigned long last_debounce_time = 0;
unsigned long debounce_delay = 1000;

void setup() {
  Serial.begin(115200);

  pinMode(SHARP_PIN, INPUT_PULLUP);

  pinMode(OCTAVE_UP_PIN, INPUT_PULLUP);
  pinMode(OCTAVE_DOWN_PIN, INPUT_PULLUP);

  pinMode(SPEAKER_PIN, OUTPUT);

  startMozzi();
}

void updateControl() {
  int key_c = mozziAnalogRead(KEY_C_PIN);
  int key_d = mozziAnalogRead(KEY_D_PIN);
  int key_e = mozziAnalogRead(KEY_E_PIN);
  int key_f = mozziAnalogRead(KEY_F_PIN);
  int key_g = mozziAnalogRead(KEY_G_PIN);
  int key_a = mozziAnalogRead(KEY_A_PIN);
  int key_b = mozziAnalogRead(KEY_B_PIN);
  int key_cc = mozziAnalogRead(KEY_CC_PIN);

  bool sharp = !digitalRead(SHARP_PIN);

  bool octave_up = !digitalRead(OCTAVE_UP_PIN);
  bool octave_down = !digitalRead(OCTAVE_DOWN_PIN);
  unsigned long current_time = millis();

  if ((current_time - last_debounce_time) > debounce_delay) {
    if (octave_up && current_octave < 5) {
      Serial.print("UP");
      Serial.print(current_octave);
      current_octave++;
      Serial.print(current_octave);
      last_debounce_time = current_time;
    }
    if (octave_down && current_octave > 3) {
      Serial.print("DOWN");
      Serial.print(current_octave);
      current_octave--;
      Serial.print(current_octave);
      last_debounce_time = current_time;
    }
  }

  if (key_c > 700) {
    if (sharp) {
      cOscil.setFreq((int)(NOTE_C[current_octave - 3] * 1.059463094359));
    }
    else {
      cOscil.setFreq(NOTE_C[current_octave - 3]);
    }
  }
  else {
    cOscil.setFreq(0);
  }

  if (key_d > 700) {
    if (sharp) {
      dOscil.setFreq((int)(NOTE_D[current_octave - 3] * 1.059463094359));
    }
    else {
      dOscil.setFreq(NOTE_D[current_octave - 3]);
    }
  }
  else {
    dOscil.setFreq(0);
  }

  if (key_e > 700) {
    eOscil.setFreq(NOTE_E[current_octave - 3]);
  }
  else {
    eOscil.setFreq(0);
  }

  if (key_f > 700) {
    if (sharp) {
      fOscil.setFreq((int)(NOTE_F[current_octave - 3] * 1.059463094359));
    }
    else {
      fOscil.setFreq(NOTE_F[current_octave - 3]);
    }
  }
  else {
    fOscil.setFreq(0);
  }

  if (key_g > 700) {
    if (sharp) {
      gOscil.setFreq((int)(NOTE_G[current_octave - 3] * 1.059463094359));
    }
    else {
      gOscil.setFreq(NOTE_G[current_octave - 3]);
    }
  }
  else {
    gOscil.setFreq(0);
  }

  if (key_a > 700) {
    if (sharp) {
      aOscil.setFreq((int)(NOTE_A[current_octave - 3] * 1.059463094359));
    }
    else {
      aOscil.setFreq(NOTE_A[current_octave - 3]);
    }
  }
  else {
    aOscil.setFreq(0);
  }

  if (key_b > 700) {
    bOscil.setFreq(NOTE_B[current_octave - 3]);
  }
  else {
    bOscil.setFreq(0);
  }

  if (key_cc > 700) {
    if (sharp) {
      ccOscil.setFreq((int)(NOTE_CC[current_octave - 3] * 1.059463094359));
    }
    else {
      ccOscil.setFreq(NOTE_CC[current_octave - 3]);
    }
  }
  else {
    ccOscil.setFreq(0);
  }
}

int updateAudio() {
  int key_c = mozziAnalogRead(KEY_C_PIN);
  int key_d = mozziAnalogRead(KEY_D_PIN);
  int key_e = mozziAnalogRead(KEY_E_PIN);
  int key_f = mozziAnalogRead(KEY_F_PIN);
  int key_g = mozziAnalogRead(KEY_G_PIN);
  int key_a = mozziAnalogRead(KEY_A_PIN);
  int key_b = mozziAnalogRead(KEY_B_PIN);
  int key_cc = mozziAnalogRead(KEY_CC_PIN);

  int c_sound = 0;
  int d_sound = 0;
  int e_sound = 0;
  int f_sound = 0;
  int g_sound = 0;
  int a_sound = 0;
  int b_sound = 0;
  int cc_sound = 0;

  if (key_c > 700) {
    c_sound = cOscil.next();
  }

  if (key_d > 700) {
    d_sound = dOscil.next();
  }

  if (key_e > 700) {
    e_sound = eOscil.next();
  }
  
  if (key_f > 700) {
    f_sound = fOscil.next();
  }

  if (key_g > 700) {
    g_sound = gOscil.next();
  }

  if (key_a > 700) {
    a_sound = aOscil.next();
  }

  if (key_b > 700) {
    b_sound = bOscil.next();
  }

  if (key_cc > 700) {
    cc_sound = ccOscil.next();
  }

  return (c_sound + d_sound + e_sound + f_sound + g_sound + a_sound + b_sound + cc_sound) / 3;
}

void loop() {
  audioHook();
}
