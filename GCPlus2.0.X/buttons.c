/*
MIT License

Copyright (c) 2020 Aurelio Mannara

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

buttons.c: Handles buttons input and debouncing. Also generates the correct answer to the poll command taking into account the requested analog mode.
*/

#include "main.h"
#include "sticks_lut.h"

#define DEBOUNCE 10 //ms

//Handy macros
#define CHECK_TIMER(b) if (toggledButtons.##b) buttonsTimers[BUTTON_##b##_ID] = 0;

#define CHECK_DEBOUNCE_BYTE0(b) if (buttonsTimers[BUTTON_##b##_ID] >= DEBOUNCE && !prevButtons.##b) { \
        buttonsTimers[BUTTON_##b##_ID] = DEBOUNCE; \
        outButtons.byte0 |= buttonsMapByte0[BUTTON_##b##_ID]; \
    }

#define CHECK_DEBOUNCE_BYTE1(b) if (buttonsTimers[BUTTON_##b##_ID] >= DEBOUNCE && !prevButtons.##b) { \
        buttonsTimers[BUTTON_##b##_ID] = DEBOUNCE; \
        outButtons.byte1 |= buttonsMapByte1[BUTTON_##b##_ID]; \
    }

#define CHECK_DEBOUNCE(b) CHECK_DEBOUNCE_BYTE0(b) \
    CHECK_DEBOUNCE_BYTE1(b)

inButtons_t prevButtons;
outButtons_t outButtons;
origins_t origins;

uint8_t buttonsTimers[N_BUTTONS];
uint8_t buttonsMessage[10];
uint8_t buttonsMapByte0[N_BUTTONS];
uint8_t buttonsMapByte1[N_BUTTONS];

void buttonsInit(void) {
    //Use timer 0 as a 1 ms timer for debouncing
    T0CON0 = 0x00;
    T0CON1 = 0x47; //FOSC/4 (16MHz). Prescaler = 1:128
    TMR0H = 124; //Rollover at 124 ticks
    TMR0IF = 0;
    T0CON0 = 0x80; //T0ON = 1. 8bit. 1:1 postscaler

    //Default mapping
    memset(buttonsMapByte0, 0, N_BUTTONS);
    memset(buttonsMapByte1, 0, N_BUTTONS);
    //Byte 0
    buttonsMapByte0[BUTTON_A_ID] = 0x01;
    buttonsMapByte0[BUTTON_B_ID] = 0x02;
    buttonsMapByte0[BUTTON_X_ID] = 0x04;
    buttonsMapByte0[BUTTON_Y_ID] = 0x08;
    buttonsMapByte0[BUTTON_ST_ID] = 0x10;
    //Byte 1
    buttonsMapByte1[BUTTON_DL_ID] = 0x01;
    buttonsMapByte1[BUTTON_DR_ID] = 0x02;
    buttonsMapByte1[BUTTON_DD_ID] = 0x04;
    buttonsMapByte1[BUTTON_DU_ID] = 0x08;
    buttonsMapByte1[BUTTON_Z_ID] = 0x10;
    buttonsMapByte1[BUTTON_RD_ID] = 0x20;
    buttonsMapByte1[BUTTON_LD_ID] = 0x40;
}

void buttonsUpdate(void) {
    uint8_t i;
    uint8_t PA, PB, PC;
    inButtons_t toggledButtons;

    //Check which pins were toggled
    PA = PORTA;
    PB = PORTB;
    PC = PORTC;
    toggledButtons.PORTA = (uint8_t)(PA ^ prevButtons.PORTA);
    toggledButtons.PORTB = (uint8_t)(PB ^ prevButtons.PORTB);
    toggledButtons.PORTC = (uint8_t)(PC ^ prevButtons.PORTC);
    //Store values for next frame
    prevButtons.PORTA = PA;
    prevButtons.PORTB = PB;
    prevButtons.PORTC = PC;

    //Reset the timers if the pins were toggled
    CHECK_TIMER(A)
    CHECK_TIMER(B)
    CHECK_TIMER(X)
    CHECK_TIMER(Y)
    CHECK_TIMER(ST)
    CHECK_TIMER(DL)
    CHECK_TIMER(DR)
    CHECK_TIMER(DD)
    CHECK_TIMER(DU)
    CHECK_TIMER(Z)
    CHECK_TIMER(RD)
    CHECK_TIMER(LD)
    CHECK_TIMER(LA)
    CHECK_TIMER(RA)
    CHECK_TIMER(Z2)

    if (!TMR0IF) return;
    TMR0IF = 0; //Reset the timer interrupt flag

    //Increment the timers
    for (i = 0; i < N_BUTTONS; i++) {
        buttonsTimers[i]++;
    }

    outButtons.byte0 = 0x00;
    outButtons.byte1 = 0x80; //Make sure er is 1
    //Update valid buttons
    CHECK_DEBOUNCE(A)
    CHECK_DEBOUNCE(B)
    CHECK_DEBOUNCE(X)
    CHECK_DEBOUNCE(Y)
    CHECK_DEBOUNCE(ST)
    CHECK_DEBOUNCE(DL)
    CHECK_DEBOUNCE(DR)
    CHECK_DEBOUNCE(DD)
    CHECK_DEBOUNCE(DU)
    CHECK_DEBOUNCE(Z)
    CHECK_DEBOUNCE(RD)
    CHECK_DEBOUNCE(LD)
    CHECK_DEBOUNCE(Z2)
    //Slightly different check for the analog triggers
    if (buttonsTimers[BUTTON_LA_ID] >= DEBOUNCE) {
        buttonsTimers[BUTTON_LA_ID] = DEBOUNCE;
        if (prevButtons.LA) {
            outButtons.LA = 0;
        } else {
            outButtons.LA = 0xFF;
        }
    }
    if (buttonsTimers[BUTTON_RA_ID] >= DEBOUNCE) {
        buttonsTimers[BUTTON_RA_ID] = DEBOUNCE;
        if (prevButtons.RA) {
            outButtons.RA = 0;
        } else {
            outButtons.RA = 0xFF;
        }
    }
}

uint8_t* buttonsGetMessage(uint8_t analogMode) {
    uint8_t correction_type;
    uint8_t cx, cy;
    buttonsMessage[0] = outButtons.byte0;
    buttonsMessage[1] = outButtons.byte1;
    // Convert to first quadrant only
    correction_type = sticks_lut[ADC_SX + 0x7F][ADC_SY + 0x7F];
    // Correct main stick
    if (correction_type & MAP_ANGLE_HORIZ) {
        buttonsMessage[2] = ADC_SX >= 0x80 ? ANGLE_HORIZ_X : -ANGLE_HORIZ_X;
        buttonsMessage[3] = ADC_SY >= 0x80 ? ANGLE_HORIZ_Y : -ANGLE_HORIZ_Y;
    } else if (correction_type & MAP_ANGLE_VERT) {
        buttonsMessage[2] = ADC_SX >= 0x80 ? ANGLE_VERT_X : -ANGLE_VERT_X;
        buttonsMessage[3] = ADC_SY >= 0x80 ? ANGLE_VERT_Y : -ANGLE_VERT_Y;
    } else if (correction_type & MAP_SHIELD_DROP) {
        buttonsMessage[2] = ADC_SX >= 0x80 ? SHIELD_DROP_X : -SHIELD_DROP_X;
        buttonsMessage[3] = ADC_SY >= 0x80 ? SHIELD_DROP_Y : -SHIELD_DROP_Y;
    } else {
        buttonsMessage[2] = ADC_SX;
        buttonsMessage[3] = ADC_SY;
    }
    // Correct c-stick
    if (correction_type & MAP_C_HORIZ) {
        cx = ADC_CX >= 0x80 ? C_HORIZ_X : -C_HORIZ_X;
        cy = C_HORIZ_Y;
    } else if (correction_type & MAP_C_VERT) {
        cx = C_HORIZ_X;
        cy = ADC_CY >= 0x80 ? C_HORIZ_Y : -C_HORIZ_Y;
    } else {
        cx = ADC_CX;
        cy = ADC_CY;
    }

    switch (analogMode) {
        case 0:
        case 5:
        case 6:
        case 7:
            buttonsMessage[4] = cx;
            buttonsMessage[5] = cy;
            //I swear Microchip, I hate you
            buttonsMessage[6] = (uint8_t)((uint8_t)(ADC_L & 0xF0U) | (uint8_t)(ADC_R >> 4U));
            buttonsMessage[7] = 0x00; //Analog A/B
        break;

        case 1:
            buttonsMessage[4] = (uint8_t)((uint8_t)(cx & 0xF0U) | (uint8_t)(cy >> 4U));
            buttonsMessage[5] = ADC_L;
            buttonsMessage[6] = ADC_R;
            buttonsMessage[7] = 0x00; //Analog A/B
        break;

        case 2:
            buttonsMessage[4] = (uint8_t)((uint8_t)(cx & 0xF0U) | (uint8_t)(cy >> 4U));
            buttonsMessage[5] = (uint8_t)((uint8_t)(ADC_L & 0xF0U) | (uint8_t)(ADC_R >> 4U));
            buttonsMessage[6] = 0x00; //Analog A
            buttonsMessage[7] = 0x00; //Analog B
        break;

        case 3:
            buttonsMessage[4] = cx;
            buttonsMessage[5] = cy;
            buttonsMessage[6] = ADC_L;
            buttonsMessage[7] = ADC_R;
        break;

        case 4:
            buttonsMessage[4] = cx;
            buttonsMessage[5] = cy;
            buttonsMessage[6] = 0x00; //Analog A
            buttonsMessage[7] = 0x00; //Analog B
        break;
    }

    return buttonsMessage;
}

void buttonsSetOrigins(void) {
    origins.SX = ADC_SX;
    origins.SY = ADC_SY;
    origins.CX = ADC_CX;
    origins.CY = ADC_CY;
    origins.L = ADC_L;
    origins.R = ADC_R;
}

uint8_t* buttonsGetOrigins(void) {
    buttonsMessage[0] = 0x00;
    buttonsMessage[1] = 0x00;
    buttonsMessage[2] = origins.SX;
    buttonsMessage[3] = origins.SY;
    buttonsMessage[4] = origins.CX;
    buttonsMessage[5] = origins.CY;
    buttonsMessage[6] = origins.L;
    buttonsMessage[7] = origins.R;
    buttonsMessage[8] = 0x02;
    buttonsMessage[9] = 0x02;
    
    return buttonsMessage;
}

void buttonsSetMapByte0(uint8_t* map) {
    uint8_t i;

    for (i = 0; i < N_BUTTONS; i++) {
        buttonsMapByte0[i] = map[i];
    }
}

void buttonsSetMapByte1(uint8_t* map) {
    uint8_t i;

    for (i = 0; i < N_BUTTONS; i++) {
        buttonsMapByte1[i] = map[i];
    }
}

uint8_t* buttonsGetMapByte0(void) {
    uint8_t i;

    for (i = 0; i < N_BUTTONS; i++) {
        buttonsMessage[i] = buttonsMapByte0[i];
    }

    return buttonsMessage;
}

uint8_t* buttonsGetMapByte1(void) {
    uint8_t i;

    for (i = 0; i < N_BUTTONS; i++) {
        buttonsMessage[i] = buttonsMapByte1[i];
    }

    return buttonsMessage;
}
