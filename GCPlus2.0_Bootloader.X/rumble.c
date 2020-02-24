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

rumble.c: Handles the DRV2603 for rumble
*/

#include "main.h"

void rumbleInit(void) {
    T4CON = 0x00;
    T4CLKCON = 0x01; //FOSC/4 (16MHz)
    T4HLT = 0x00; //Software gate control
    T4PR = 0xFF; //62.5 kHz, 8bit resolution
    T4TMR = 0x00;
    T4CON = 0x80;

    CCPTMRS0bits.C1TSEL = 2; //Timer4 to CCP1
    CCPR1L = 0x00;
    CCPR1H = 0x80; //50% => 0V
    CCP1CON = 0x9C; //Enabled. Left-aligned PWM
}

void rumbleSpin(uint8_t speed) {
    LATB4 = 1; //Enable DRV2603
    CCPR1H = speed;
}

void rumbleBrake(void) {
    LATB4 = 1; //Enable DRV2603
    CCPR1H = 0x60;
}

void rumbleStop(void) {
    LATB4 = 0; //Disable DRV2603
    CCPR1H = 0x80;
}
