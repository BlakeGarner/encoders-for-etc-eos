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

#pragma once
#include <Arduino.h>

/**
 * Configures LEDs (button & RGB) as required.
 */
void setup_leds();

/**
 * Sets a particular knob LED to the specified colour.
 * @param ledNum: Which knob LED to adjust.
 * @param colour: The colour to set it to, specified as 0x00RRGGBB
 */
void setKnobLED(int ledNum, uint32_t colour);

/**
 * Sets a particular status LED to the specified colour.
 * @param ledNum: Which status LED to adjust.
 * @param colour: The colour to set it to, specified as 0x00RRGGBB.
 */
void setStatusLED(int ledNum, uint32_t colour);

/**
 * Sets a particular button LED to the specified intensity.
 * @param ledNum: Which button LED to adjust.
 * @param intensity: The intesity to set it to, in the range 0 to 255.
 */
void setButtonLED(int ledNum, uint8_t intensity);

static const uint32_t LED_RED = 0xFF0000;
static const uint32_t LED_ORANGE  = 0xFF4400;
static const uint32_t LED_AMBER   = 0xFF8800;
static const uint32_t LED_YELLOW  = 0xFFBB00;
static const uint32_t LED_LIME    = 0xAAFF66;
static const uint32_t LED_GREEN   = 0x00FF00;
static const uint32_t LED_TURQ    = 0x00FF66;
static const uint32_t LED_CYAN    = 0x00FFFF;
static const uint32_t LED_SKYBLUE = 0x0066FF;
static const uint32_t LED_BLUE    = 0x0000FF;
static const uint32_t LED_PURPLE  = 0xA000FF;
static const uint32_t LED_MAGENTA = 0xFF00FF;
static const uint32_t LED_WHITE   = 0xFFFFFF;
static const uint32_t LED_GREY    = 0x808080;
static const uint32_t LED_BLACK   = 0x000000;
static const uint32_t LED_CTO     = 0xFFBB88;
static const uint32_t LED_CTB     = 0xFFFFFF;

static const int CONNECTED_STATUS_LED = 0;
