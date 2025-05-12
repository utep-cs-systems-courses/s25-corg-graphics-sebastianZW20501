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
void wake_up();
void switch_interrupt_handler();
void configure_clocks();
void clear_screen();

// Switch Interrupt Handler
void __interrupt_vec(PORT2_VECTOR) Port_2() {
    if (P2IFG & SWITCHES) {
        P2IFG &= ~SWITCHES;
        switch_interrupt_handler();
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
    if (step == 0) {
        clear_screen();
    } else {
        // Draw hourglass logic (replace with your own)
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


// Wake Up Logic
void wake_up() {
    __bic_SR_register_on_exit(LPM4_bits); // Exit LPM4
}

// Main Entry Point
void main() {
    configure_clocks();
    P1DIR |= LED;
    P1OUT |= LED;
    currentState = STATE_SLEEP;
    go_to_sleep();
    while (1) {
        if (redrawScreen) {
            redrawScreen = 0;
            update_shape();
        }
    }
}

