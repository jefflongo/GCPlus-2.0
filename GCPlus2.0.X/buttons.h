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

buttons.h: Handles buttons input and debouncing. Also generates the correct answer to the poll command taking into account the requested analog mode.
*/

#ifndef _BUTTONS_H_
#define _BUTTONS_H_

typedef union {
    struct {
        //PORTA
        unsigned : 2;
        unsigned LD : 1;
        unsigned DL : 1;
        unsigned DU : 1;
        unsigned DR : 1;
        unsigned ST : 1;
        unsigned DD : 1;

        // PORTB
        unsigned Z : 1;
        unsigned RD : 1;
        unsigned Y : 1;
        unsigned X : 1;
        unsigned : 1;
        unsigned Z2 : 1;
        unsigned : 2;

        // PORTC
        unsigned LA : 1;
        unsigned : 4;
        unsigned RA : 1;
        unsigned B : 1;
        unsigned A : 1;
    };
    struct {
        uint8_t PORTA;
        uint8_t PORTB;
        uint8_t PORTC;
    };
} inButtons_t;

typedef union {
    struct {
        unsigned A  : 1;
        unsigned B  : 1;
        unsigned X  : 1;
        unsigned Y  : 1;
        unsigned ST : 1;
        unsigned    : 3;
        unsigned DL : 1;
        unsigned DR : 1;
        unsigned DD : 1;
        unsigned DU : 1;
        unsigned Z  : 1;
        unsigned RD : 1;
        unsigned LD : 1;
        unsigned er : 1;
        uint8_t LA;
        uint8_t RA;
    };
    struct
    {
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte6;
        uint8_t byte7;
    };
} outButtons_t;

typedef struct {
    uint8_t SX;
    uint8_t SY;
    uint8_t CX;
    uint8_t CY;
    uint8_t L;
    uint8_t R;
} origins_t;

enum {
    BUTTON_A_ID = 0,
    BUTTON_B_ID,
    BUTTON_X_ID,
    BUTTON_Y_ID,
    BUTTON_ST_ID,
    BUTTON_DL_ID,
    BUTTON_DR_ID,
    BUTTON_DD_ID,
    BUTTON_DU_ID,
    BUTTON_Z_ID,
    BUTTON_RD_ID,
    BUTTON_LD_ID,
    BUTTON_LA_ID,
    BUTTON_RA_ID,
    BUTTON_Z2_ID,
    N_BUTTONS
} ButtonsID;

void buttonsInit(void);
void buttonsUpdate(void);
uint8_t* buttonsGetMessage(uint8_t analogMode);
void buttonsSetOrigins();
uint8_t* buttonsGetOrigins(void);
void buttonsSetMapByte0(uint8_t* map);
void buttonsSetMapByte1(uint8_t* map);
uint8_t* buttonsGetMapByte0(void);
uint8_t* buttonsGetMapByte1(void);

#endif
