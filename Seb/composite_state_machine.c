#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "msquares.c"

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

// Ball Movement
short drawPos[2] = {1, 10}, controlPos[2] = {2, 10};
short colVelocity = 1, colLimits[2] = {1, 48};

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


void switch_interrupt_handler() {
    char p2val = P2IN & SWITCHES;
    if (p2val & SW1) currentState = STATE_HOURGLASS;
    if (p2val & SW2) currentState = STATE_BALL;
    if (p2val & SW4) go_to_sleep();
    redrawScreen = 1;
}

// Draw Ball
void draw_ball(int col, int row, unsigned short color) {
    // Replace this with your fillRectangle function
}

// Update Ball Screen
void screen_update_ball() {
    for (char axis = 0; axis < 2; axis++) 
        if (drawPos[axis] != controlPos[axis]) 
            goto redraw;
    return;
redraw:
    draw_ball(drawPos[0], drawPos[1], 0x001F); // Erase (replace with your color code)
    for (char axis = 0; axis < 2; axis++) 
        drawPos[axis] = controlPos[axis];
    draw_ball(drawPos[0], drawPos[1], 0xFFFF); // Draw (replace with your color code)
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
        case STATE_BALL:
            screen_update_ball();
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
        __bis_SR_register(LPM0_bits + GIE); // Enter LPM0 (CPU off, peripherals on)
    }
}

