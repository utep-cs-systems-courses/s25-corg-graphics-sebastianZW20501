#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "stateMachine.h"
#include "sound.h"


#define SWITCHES 15
#define SW1 1 
#define SW2 2
#define SW3 4
#define SW4 8

int redrawScreen = 1;
int systemAwake = 0;

void switch_interrupt_handler()
{
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHES);
  P2IES &= (p2val | ~SWITCHES);
  char switches = ~p2val & SWITCHES;

  if (!systemAwake) {
    systemAwake = 1;
    enableWDTInterrupts(); // wake system
  }

  if (switches & SW1) current_state = 0;
  if (switches & SW2) current_state = 1;
  if (switches & SW3) current_state = 2;
  if (switches & SW4) current_state = 3;

  redrawScreen = 1;
}

void wdt_c_handler()
{
  static int blinkCount = 0;
  if (++blinkCount >= 25) {
    if (redrawScreen) {
      redrawScreen = 0;
      state_advance();
    }
    blinkCount = 0;
  }
}

void main()
{
  configureClocks();
  lcd_init();
  switch_init();
  buzzer_init();

  clearScreen(COLOR_BLACK);
  or_sr(0x8); // enable interrupts, sleep initially
  while (1) {
    if (systemAwake && redrawScreen) {
      redrawScreen = 0;
      state_advance();
    }
    or_sr(0x10); // CPU OFF
  }
}

void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {
    P2IFG &= ~SWITCHES;
    switch_interrupt_handler();
  }
}

