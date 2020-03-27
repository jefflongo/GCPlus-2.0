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

main.h: Main code. Handles ports initialization and SI commands.
*/

#ifndef _MAIN_H_
#define _MAIN_H_

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "nvm.h"
#include "config.h"
#include "adc.h"
#include "buttons.h"
#include "si.h"
// #include "rumble.h"

#define GCP2_VERSION_MAJ 2U
#define GCP2_VERSION_MIN 0U
#define GCP2_VERSION (((GCP2_VERSION_MAJ & 0x1FFF) << 3) | (GCP2_VERSION_MIN & 0x0007))

#define GCP2_HWVERSION_MAJ 2U
#define GCP2_HWVERSION_MIN 0U
#define GCP2_HWVERSION (((GCP2_HWVERSION_MAJ & 0x1FFF) << 3) | (GCP2_HWVERSION_MIN & 0x0007))

#define PAYLOAD_ADDR 0x2000 //4096 words reserved to the bootloader

#define ABS(n) ((n >= 0) ? (n) : (-n))

/*! \fn void portsInit(void)
    \brief initialize GPIO ports and PPS to the proper state
*/
void portsInit(void);

/*! \fn void bootBootloader(void)
    \brief cleans up the stack, sets the new interrupt base address and jumps to the bootloader address
*/
void bootBootloader(void);

#endif
