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
#include <String.h>
#include "hardware.h"

static const String BLANK_LABEL = String(" ");

class KnobState {
public:
    KnobState();
    void setLabel(String newLabel);
    void setInactive();
    void setValue(float newValue);
    void setValue(String newValue);
    void setColour(uint32_t colour);
    bool isDirty();
    bool getFineMode();
    void setFineMode(bool fineMode);
    char* getOledText();
    uint32_t getColour();
private:
    String _label;
    bool _isActive;
    float _fValue;
    String _sValue;
    bool _isDirty;
    bool _fineMode;
    bool _useFloatValue; //TODO: Switch to enum.
    uint32_t _colour;
    char _oledText[OLED_MAX_TEXT_LEN];
};
