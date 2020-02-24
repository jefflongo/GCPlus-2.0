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

adc.h: Implements automatic scanning of ADC channels through DMA
*/

#ifndef _ADC_H_
#define _ADC_H_

#define ADC_NCHANNELS 6

extern uint8_t ADCValues[ADC_NCHANNELS];
#define ADC_SX  ADCValues[0]
#define ADC_SY  ADCValues[1]
#define ADC_CX  ADCValues[2]
#define ADC_CY  ADCValues[3]
#define ADC_R   ADCValues[4]
#define ADC_L   ADCValues[5]

void ADCInit(uint8_t sxCh, uint8_t syCh, uint8_t cxCh, uint8_t cyCh);

#endif
