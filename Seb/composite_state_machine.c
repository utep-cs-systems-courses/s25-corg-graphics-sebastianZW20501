#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"


// State Definitions
typedef enum {
    STATE_SLEEP,
    STATE_IDLE,
    STATE_HOURGLASS,
    STATE_BALL
} State;

State currentState = STATE_SLEEP;

// Switches
#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8
#define SWITCHES 15

// LED and Timer
#define LED BIT6
short redrawScreen = 1;



// Hourglass Colors
char blue = 31, green = 0, red = 31;
unsigned char step = 0;

// Forward Declarations
void update_shape();
void draw_ball(int col, int row, unsigned short color);
void screen_update_ball();
void screen_update_hourglass();
void go_to_sleep();

void switch_interrupt_handler();
//void configure_clocks();
//void clearScreen();

// Switch Interrupt Handler
void __interrupt_vec(PORT2_VECTOR) Port_2() {
    if (P2IFG & SWITCHES) {
        P2IFG &= ~SWITCHES;
        switch_interrupt_handler();

        P1OUT ^= LED;

        // Wake up the CPU from low power mode
        __bic_SR_register_on_exit(LPM4_bits);
    }
}



void switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}



switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}



void switch_interrupt_handler() {
    char p2val = P2IN & SWITCHES;
    if (p2val & SW1) currentState = STATE_HOURGLASS;
    if (p2val & SW2) currentState = STATE_BALL;
    if (p2val & SW4) go_to_sleep();
    redrawScreen = 1;
}



// Update Hourglass Screen
void screen_update_hourglass() {
    clearScreen(COLOR_BLUE);
    
    static unsigned char row = screenHeight / 2, col = screenWidth / 2;
    static char lastStep = 0;
  
    for (lastStep = 0; lastStep <= 30; lastStep++) {
        int startCol = col - lastStep;
        int endCol = col + lastStep;
        int width = 1 + endCol - startCol;

        unsigned int color = (blue << 11) | (green << 5) | red;
        
        fillRectangle(startCol, row + lastStep, width, 1, color);
        fillRectangle(startCol, row - lastStep, width, 1, color);
    }
}


// Main Update Shape Logic
void update_shape() {
    switch (currentState) {
        case STATE_HOURGLASS:
            screen_update_hourglass();
            break;
      
        case STATE_SLEEP:
            go_to_sleep();
            break;
        default:
            break;
    }
}

// Enter Low Power Mode
void go_to_sleep() {
    or_sr(0x18); // CPU Off, GIE enabled
}




// Main Entry Point
void main() {
    // Initialize the clock and the LED
    configureClocks();
    P1DIR |= LED;   // Set LED as output
    P1OUT |= LED;   // Turn it on to indicate startup
    
    // Initialize the screen and the buttons
    lcd_init();
    clearScreen(COLOR_BLUE);
    switch_init();
    
    
    // Set the starting state to sleep
    currentState = STATE_SLEEP;

    // Enable interrupts and low-power mode
    enableWDTInterrupts();    // Watchdog timer interrupts
    or_sr(0x18);              // GIE (Global Interrupt Enable) + CPU off
    
    while (1) {
        if (redrawScreen) {
            redrawScreen = 0;
            update_shape();
        }
        
        // Low power mode wait
        //__bis_SR_register(LPM0_bits + GIE); // Enter LPM0 (CPU off, peripherals on)
    }
}

