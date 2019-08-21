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

#include "knobConfig.h"
#include <Arduino.h>
#include <string.h>

KnobConfig::KnobConfig() : KnobConfig(0, 0, "", "") { }

KnobConfig::KnobConfig(
    uint8_t type,
    uint32_t colour,
    const char* label,
    const char* oscDesc,
    float coarseClicks,
    float fineClicks
) {
    _type = type;
    _colour = colour & 0xFFFFFF;
    _label = String(label);
    //_label.copy(label, MAX_STR_LENGTH);
    _oscDesc = String(oscDesc);
    //_oscDesc.copy(oscDesc, MAX_STR_LENGTH);
    _coarseClicks = coarseClicks;
    _fineClicks = fineClicks;
}

uint8_t KnobConfig::getType() {
    return _type;
}

uint32_t KnobConfig::getColour() {
    return _colour;
}

String KnobConfig::getLabel() {
    return _label;
}

String KnobConfig::getOscDesc() {
    return _oscDesc;
}

float KnobConfig::getCoarseClicks() {
    return _coarseClicks;
}

float KnobConfig::getFineClicks() {
    return _fineClicks;
}

bool KnobConfig::isBlank() {
    return (_oscDesc.length() == 0);
}