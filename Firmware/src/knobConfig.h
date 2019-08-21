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


class KnobConfig {
public:
    KnobConfig();
    
    KnobConfig(
        uint8_t type,
        uint32_t colour,
        const char* label,
        const char* oscDesc,
        float coarseClicks = 1.0,
        float fineClicks = 1.0
    );
    uint8_t getType();
    uint32_t getColour();
    String getLabel();
    String getOscDesc();
    float getCoarseClicks();
    float getFineClicks();
    bool isBlank();
private:
    uint8_t _type;
    uint32_t _colour;
    String _label;
    String _oscDesc;
    float _coarseClicks;
    float _fineClicks;
    static const int MAX_STR_LENGTH = 50;
};
