#include <msp430.h>
#include "sound.h"

void buzzer_init(){
  P2DIR |= BIT6;
  P2SEL |= BIT6;

  TA0CCR0 = 1000;
  TA0CCTL1 = OUTMOD_7;
  TA0CCR1 = 500;
  TA0CTL = TASSEL_2 + MC_1;
}

void play_sound(int freq) {
  if (freq == 0) {
    stop_sound();
    return;
  }
  TA0CCR0 = 2000000 / freq;
  TA0CCR1 = TA0CCR0 / 2;
}

void stop_sound() {
  TA0CCR1 = 0;
}

