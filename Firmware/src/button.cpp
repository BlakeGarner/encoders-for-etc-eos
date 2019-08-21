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

#include "button.h"

Button::Button() : Button(false) { }

Button::Button(bool startingState) {
    _isPushed = startingState;
    _lastIsPushed = _isPushed;
}

void Button::processNewReading(bool buttonState) {
    _isPushed = buttonState;
}

bool Button::isPushed() {
    return _isPushed;
}

bool Button::wasPushed() {
    if(_isPushed && !_lastIsPushed) {
        _lastIsPushed = true;
        return true;
    }
    else {
        _lastIsPushed = _isPushed;
        return false;
    }
}

bool Button::wasReleased() {
    if(!_isPushed && !_lastIsReleased) {
        _lastIsReleased = true;
        return true;
    }
    else {
        _lastIsReleased = !_isPushed;
        return false;
    }
}