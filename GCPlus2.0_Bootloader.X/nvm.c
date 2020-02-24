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

nvm.c: Implements functions to handle internal NVM memories.
*/

#include "main.h"

void NVMUnlock(void) {
    INTCON0bits.GIE = 0;
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1;
    INTCON0bits.GIE = 1;
}

void PGMEraseRow(uint16_t addr) {
    if (addr & 63)
        return;
    if (addr < PAYLOAD_ADDR || (addr + 64) > 0x8000)
        return;

    NVMCON1 = 0;
    TBLPTRU = 0;
    TBLPTRH = (addr >> 8) & 0xFF;
    TBLPTRL = addr & 0xFF;
    NVMCON1bits.REG0 = 0;
    NVMCON1bits.REG1 = 1;
    NVMCON1bits.FREE = 1;
    NVMCON1bits.WREN = 1;
    NVMUnlock();
    NVMCON1bits.WREN = 0;
}

void PGMWriteBlock(uint16_t addr, uint8_t* data) {
    if (addr & 63)
        return;
    if (addr < PAYLOAD_ADDR || (addr + 64) > 0x8000)
        return;

    uint16_t i;

    /*NVMCON1 = 0;

    //Load address
    TBLPTRU = 0;
    TBLPTRH = (addr >> 8) & 0xFF;
    TBLPTRL = addr & 0xFF;*/

    PGMEraseRow(addr);

    //Copy block to the latches
    asm("tblrd*-");
    for (i = 0; i < 64; i++) {
        TABLAT = data[i];
        asm("tblwt+*");
    }
    //Write block
    NVMCON1bits.REG0 = 0;
    NVMCON1bits.REG1 = 1;
    NVMCON1bits.WREN = 1;
    NVMCON1bits.FREE = 0;

    NVMUnlock();

    NVMCON1bits.WREN = 0;
}

void PGMReadBlock(uint16_t addr, uint8_t* data) {
    if (addr & 63)
        return;
    if (addr < PAYLOAD_ADDR || (addr + 64) > 0x8000)
        return;

    uint16_t i;

    //Load address
    TBLPTRU = 0;
    TBLPTRH = (addr >> 8) & 0xFF;
    TBLPTRL = addr & 0xFF;

    //Select PGM
    NVMCON1bits.REG0 = 0;
    NVMCON1bits.REG1 = 1;

    //Read block from the latches
    for (i = 0; i < 64; i++) {
        asm("tblrd*+");
        data[i] = TABLAT;
    }
}

uint8_t EEPROMReadByte(uint8_t address) {
    NVMCON1 = 0;
    NVMADRL = address;
    NVMCON1bits.RD = 1;
    return NVMDAT;
}

void EEPROMWriteByte(uint8_t address, uint8_t data) {
    NVMCON1 = 0;
    NVMADRL = address;
    NVMDAT = data;
    NVMCON1bits.WREN = 1;
    NVMUnlock();
    while (NVMCON1bits.WR);
    NVMCON1bits.WREN = 0;
}
