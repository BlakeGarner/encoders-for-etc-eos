/* 
 * Copyright 2019 Blake Garner
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "leds.h"
#include "hardware.h"
#include <WS2812Serial.h>

uint32_t lookup_rgb_red[256];
uint32_t lookup_rgb_grn[256];
uint32_t lookup_rgb_blu[256];

uint16_t lookup_butt_red[256];
uint16_t lookup_butt_amb[256];
uint16_t lookup_butt_grn[256];
uint16_t lookup_butt_blu[256];

byte ws2812_drawingMemory[NUM_WS2812*3]; // 3 bytes per LED required.
DMAMEM byte ws2812_displayMemory[NUM_WS2812*12]; // 12 bytes per LED required.
WS2812Serial ws2812_leds(
    NUM_WS2812,
    ws2812_displayMemory,
    ws2812_drawingMemory,
    WS2812_PIN,
    WS2812_GRB);

void _generateLookupTables() {
    for(int i = 0; i < 256; i++) {
        float i_float = ((float) i) / 255.0;

        lookup_rgb_red[i] 
            = round(RGB_PWM_MAX_LEVEL*RGB_RED_BRI*pow(i_float, RGB_RED_GAMMA));
        lookup_rgb_grn[i] 
            = round(RGB_PWM_MAX_LEVEL*RGB_GRN_BRI*pow(i_float, RGB_GRN_GAMMA));
        lookup_rgb_blu[i] 
            = round(RGB_PWM_MAX_LEVEL*RGB_BLU_BRI*pow(i_float, RGB_BLU_GAMMA));

        lookup_butt_red[i]
            = round(
                BUTT_PWM_MAX_LEVEL*BUTT_RED_BRI
                *pow(i_float, BUTT_RED_GAMMA)
            );
        lookup_butt_amb[i]
            = round(
                BUTT_PWM_MAX_LEVEL*BUTT_AMB_BRI
                *pow(i_float, BUTT_AMB_GAMMA)
            );
        lookup_butt_grn[i]
            = round(
                BUTT_PWM_MAX_LEVEL*BUTT_GRN_BRI
                *pow(i_float, BUTT_GRN_GAMMA)
            );
        lookup_butt_blu[i]
            = round(
                BUTT_PWM_MAX_LEVEL*BUTT_BLU_BRI
                *pow(i_float, BUTT_BLU_GAMMA)
            );
    }
}

uint32_t _correctRgbColour(uint32_t colour) {
    uint32_t result = 0;
    result  = (lookup_rgb_red[(colour >> 16) & 0xFF] << 16);
    result += (lookup_rgb_grn[(colour >> 8)  & 0xFF] << 8) ;
    result +=  lookup_rgb_blu[ colour        & 0xFF]       ;
    return result;
}

void setup_leds() {
    for(int i = 0; i < NUM_BUTTONS; i++) {
        pinMode(BUTTON_LED_PINS[i], OUTPUT);
        analogWriteFrequency(BUTTON_LED_PINS[i], BUTTON_LED_PWM_FREQ);
    }
    analogWriteResolution(BUTTON_LED_PWM_RES);

    ws2812_leds.begin();
    _generateLookupTables();
}

void _setRgbLED(int ledNum, uint32_t colour) {
    uint32_t correctedColour = _correctRgbColour(colour);
    if(ledNum >= 0 && ledNum < NUM_WS2812) {
        ws2812_leds.setPixel(ledNum, correctedColour);
        ws2812_leds.show();
    }
}

void setKnobLED(int ledNum, uint32_t colour) {
    if(ledNum >= 0 && ledNum < NUM_KNOBS) {
        _setRgbLED(KNOB_LED_INDEXES[ledNum], colour);
    }
}

void setStatusLED(int ledNum, uint32_t colour) {
    if(ledNum >= 0 && ledNum < NUM_STATUS_LEDS) {
        _setRgbLED(STATUS_LED_INDEXES[ledNum], colour);
    }
}

void setButtonLED(int ledNum, uint8_t intensity) {
    uint16_t correctedIntensity = 0;
    switch(ledNum) {
        case BUTTON_PAGE_RED_INDEX:
        case BUTTON_AUX_RED_INDEX:
            correctedIntensity = lookup_butt_red[intensity];
            break;
        case BUTTON_PAGE_AMB_INDEX:
            correctedIntensity = lookup_butt_amb[intensity];
            break;
        case BUTTON_PAGE_GRN_INDEX:
            correctedIntensity = lookup_butt_grn[intensity];
            break;
        case BUTTON_PAGE_BLU_INDEX:
        case BUTTON_AUX_BLU_INDEX:
            correctedIntensity = lookup_butt_blu[intensity];
            break;
    }

    if(ledNum >= 0 && ledNum < NUM_BUTTONS) {
        analogWrite(BUTTON_LED_PINS[ledNum], correctedIntensity);
    }
}