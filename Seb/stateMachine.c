#include "stateMachine.h"
#include "lcdutils.h"
#include "lcddraw.h"
#include "sound.h"

int current_state = 0;

void state_advance() {
  switch (current_state) {
    case 0:
      play_sound(500); // e.g., 500 Hz
      clearScreen(COLOR_RED);
      drawString5x7(20, 20, "State 1", COLOR_WHITE, COLOR_RED);
      break;
    case 1:
      play_sound(800);
      clearScreen(COLOR_GREEN);
      drawString5x7(20, 20, "State 2", COLOR_BLACK, COLOR_GREEN);
      break;
    case 2:
      play_sound(1000);
      clearScreen(COLOR_BLUE);
      drawString5x7(20, 20, "State 3", COLOR_WHITE, COLOR_BLUE);
      break;
    case 3:
      play_sound(1200);
      clearScreen(COLOR_ORANGE);
      drawString5x7(20, 20, "State 4", COLOR_BLACK, COLOR_ORANGE);
      break;
  }
}

