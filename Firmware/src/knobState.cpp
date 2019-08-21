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

#include "knobState.h"

KnobState::KnobState() {
    _label = String(BLANK_LABEL);
    _isActive = false;
    _fValue = 0.0;
    _sValue = String(BLANK_LABEL);
    _isDirty = true;
    _fineMode = false;
    _useFloatValue = true;
    _oledText[0] = '\0';
    _colour = 0;
}

void KnobState::setLabel(String newLabel) {
    if(newLabel.length() == 0) {
        if(_label != BLANK_LABEL) {
            _label = String(BLANK_LABEL);
            _fineMode = false;
            _isActive = false;
            _isDirty = true;
        }
    }
    else if(_label != newLabel) {
        _label = String(newLabel);
        _fineMode = false;
        _isActive = false;
        _isDirty = true;
    }
}

void KnobState::setInactive() {
    if(_isActive) {
        _isActive = false;
        _isDirty = true;
    }
}

void KnobState::setValue(float newValue) {
    if(!_useFloatValue || _fValue != newValue || !_isActive) {
        _useFloatValue = true;
        _fValue = newValue;
        _isActive = true;
        _isDirty = true;
    }
}

void KnobState::setValue(String newValue) {
    if(_useFloatValue) {
        _useFloatValue = false;
        _isDirty = true;
    }
    if(newValue.length() == 0) {
        if(_sValue != BLANK_LABEL) {
            _sValue = String(BLANK_LABEL);
            _isDirty = true;
        }
    }
    else if(_sValue != newValue) {
        _sValue = String(newValue);
        _isDirty = true;
    }
}

void KnobState::setColour(uint32_t colour) {
    if(_colour != colour) {
        _colour = colour;
    }
}

bool KnobState::isDirty() {
    return _isDirty;
}

bool KnobState::getFineMode() {
    return _fineMode;
}

void KnobState::setFineMode(bool fineMode) {
    if(_fineMode != fineMode) {
        _fineMode = fineMode;
        _isDirty = true;
    }
}

char* KnobState::getOledText() {
    if(_isDirty) {
        if(!_isActive) {
            strncpy(_oledText, "", OLED_MAX_TEXT_LEN);
        }
        else if(_useFloatValue) {
            snprintf(
                _oledText, 
                OLED_MAX_TEXT_LEN, 
                "%-10s\n%c%9.5f", 
                _label.c_str(),
                (_fineMode ? 'F' : ' '),
                _fValue);
        }
        else {
            snprintf(
                _oledText, 
                OLED_MAX_TEXT_LEN, 
                "%-10s\n%-10s", 
                _label.c_str(), 
                _sValue.c_str());
        }
    }
    _isDirty = false;
    return _oledText;
}

uint32_t KnobState::getColour() {
    if(_isActive) {
        return _colour;
    }
    else {
        return 0;
    }
}