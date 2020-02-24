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

config.h: Implements few functions to take care of the configuration structure in the internal EEPROM
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define DZ_MODE_RADIAL          0
#define DZ_MODE_SCALEDRADIAL    1

#define TRIG_MODE_DIGITAL   0
#define TRIG_MODE_ANALOG    1

typedef struct {
    //Header
    uint32_t magicCode; //0x322B4347 = 'GC+2'
    uint16_t version;

    //Sticks configuration
    uint8_t SXMin;
    uint8_t SXMax;
    uint8_t SYMin;
    uint8_t SYMax;
    uint8_t CXMin;
    uint8_t CXMax;
    uint8_t CYMin;
    uint8_t CYMax;
    uint8_t SXChan;
    uint8_t SYChan;
    uint8_t CXChan;
    uint8_t CYChan;
    union {
        struct {
            uint8_t SXInvert : 1;
            uint8_t SYInvert : 1;
            uint8_t CXInvert : 1;
            uint8_t CYInvert : 1;
            uint8_t : 4; //padding
        };
        uint8_t axInvert;
    };
    uint8_t SDeadzone;
    uint8_t CDeadzone;

    //0: radial. 1: scaled radial
    uint8_t deadzoneMode;

    //Rumble
    uint8_t rumbleIntensity;

    //Triggers mode: 0: digital, referred to GND. 1: analog
    uint8_t triggersMode;
} config_t;

extern config_t config;

void configInit(void);
void configSetDefault(void);
void configFlashAll(void);

#endif  /* CONFIG_H */
