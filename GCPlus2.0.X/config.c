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

config.c: Implements few functions to take care of the configuration structure in the internal EEPROM
*/

#include "main.h"

config_t config;

void configInit(void) {
    uint8_t* data = (uint8_t*)&config;
    uint8_t i;
    for (i = 0; i < sizeof(config_t); i++) {
        asm("clrwdt");
        data[i] = EEPROMReadByte(i);

        //Check magic code
        if (i == 3U && config.magicCode != 0x322B4347UL) {
            //Magic code wrong. Set default configuration
            configSetDefault();
            configFlashAll();
        }
    }
}

void configSetDefault(void) {
    //Clear all parameters
    memset((void*)&config, 0, sizeof(config_t));

    //Header
    config.magicCode = 0x322B4347UL;
    config.version = GCP2_VERSION;

    //Sticks configuration
    config.SXMin = 0x00;
    config.SXMax = 0xFF;
    config.SYMin = 0x00;
    config.SYMax = 0xFF;
    config.CXMin = 0x00;
    config.CXMax = 0xFF;
    config.CYMin = 0x00;
    config.CYMax = 0xFF;
    config.SXChan = 0x11;
    config.SYChan = 0x12;
    config.CXChan = 0x13;
    config.CYChan = 0x14;
    config.axInvert = 0x00;
    config.SDeadzone = 0x18;
    config.CDeadzone = 0x18;
    config.deadzoneMode = DZ_MODE_SCALEDRADIAL;

    //Rumble
    config.rumbleIntensity = 0xFF;

    //Triggers mode
    config.triggersMode = TRIG_MODE_ANALOG;
}

void configFlashAll(void) {
    uint8_t i;
    uint8_t* data = (uint8_t*)&config;
    for (i = 0; i < sizeof(config_t); i++) {
        EEPROMWriteByte(i, data[i]);
    }
}
