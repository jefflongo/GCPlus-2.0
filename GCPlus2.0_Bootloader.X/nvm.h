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

nvm.h: Implements functions to handle internal NVM memories.
*/

#ifndef _NVM_H_
#define _NVM_H_

void NVMUnlock(void);
void PGMEraseRow(uint16_t addr);
void PGMWriteBlock(uint16_t addr, uint8_t* data);
void PGMReadBlock(uint16_t addr, uint8_t* data);
uint8_t EEPROMReadByte(uint8_t address);
void EEPROMWriteByte(uint8_t address, uint8_t data);

#endif
