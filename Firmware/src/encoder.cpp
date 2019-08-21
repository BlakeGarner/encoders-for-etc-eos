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

#include "encoder.h"

Encoder::Encoder() : Encoder(0x40) { }

Encoder::Encoder(uint8_t startingState) {
    // Pulls in 3LSB from startingState. Bits <5:4> define
    // the direction and start as "unknown" (0b11).
    _encoderState = (startingState & 0x07) | 0x18;
    _encoderClicks = 0;
    _isPushed = (startingState & 0x04) == 0;
    _lastUpdatedEncoderClicks = _encoderClicks;
    _lastIsPushed = _isPushed;
}

// 0b00 = stopped, 0b01 = forwards, 0b10 = reverse, 0b11 = unknown.
static uint8_t _findNewDirection(uint8_t oldState, uint8_t newState) {
    //    Old     New    
    //   State   State   Result
    //   -----   -----   ------
    //0   00  ->  00  =   00 (Stopped)
    //1   00  ->  01  =   01 (Forwards)
    //2   00  ->  10  =   10 (Reverse)
    //3   00  ->  11  =   11 (Unknown)
    //4   01  ->  00  =   10 (Reverse)
    //5   01  ->  01  =   00 (Stopped)
    //6   01  ->  10  =   11 (Unknown)
    //7   01  ->  11  =   01 (Forwards)
    //8   10  ->  00  =   01 (Forwards)
    //9   10  ->  01  =   11 (Unknown)
    //10  10  ->  10  =   00 (Stopped)
    //11  10  ->  11  =   10 (Reverse)
    //12  11  ->  00  =   11 (Unknown)
    //13  11  ->  01  =   10 (Reverse)
    //14  11  ->  10  =   01 (Forwards)
    //15  11  ->  11  =   00 (Stopped)
    uint8_t combinedState = ((oldState & 0x03) << 2) | (newState & 0x03);
    // combinedState gives the old and new together in the one value for a more elegant switch statement
    switch(combinedState) {
        case 0b0000: // 0
        case 0b0101: // 5
        case 0b1010: // 10 
        case 0b1111: // 15
            return 0x00; // Stopped
        case 0b0001: // 1
        case 0b0111: // 7
        case 0b1000: // 8
        case 0b1110: // 14
            return 0x01; // Forwards
        case 0b0010: // 2
        case 0b0100: // 4
        case 0b1011: // 11
        case 0b1101: // 13
            return 0x02; // Reverse
        default:
            return 0x03; // Unknown
    } 
}

void Encoder::processNewReading(uint8_t newPosition) {
    uint8_t encoderDirection = _findNewDirection(_encoderState, newPosition);
    switch(encoderDirection) {
        case 0: // no change in encoder. Do nothing.
            break;
        case 0b01: // Forwards one click.
            if((_encoderState & 0x18) == 0x08) {
                // Only record as a click if it was already going that way.
                _encoderClicks++;
            }
            // Set state to fwd + newPosition.
            _encoderState = 0x08 | (newPosition & 0x07);
            break;
        case 0b10: // Backwards one click.
            if((_encoderState & 0x18) == 0x10) {
                // Only record as a click if it was already going that way.
                _encoderClicks--;
            }
            // Set state to rev + newPosition.
            _encoderState = 0x10 | (newPosition & 0x07);
            break;
        case 0b11: // Two clicks fwds or backwards (unknown which way).
        default:
            _encoderState = 0x18 | (newPosition & 0x07);
            break;
    }
    // TODO: Determine encoder clicks, double clicks & hold states.
    _isPushed = ((newPosition & 0x04) == 0);
}

int32_t Encoder::numClicks() {
    if(_lastUpdatedEncoderClicks != _encoderClicks) {
        int32_t result = _encoderClicks - _lastUpdatedEncoderClicks;
        _lastUpdatedEncoderClicks = _encoderClicks;
        return result;
    }
    return 0;
}

int32_t Encoder::totalNumClicks() {
    return _encoderClicks;
}

boolean Encoder::wasPushed() {
    if(_isPushed && !_lastIsPushed) {
        _lastIsPushed = true;
        return true;
    }
    else {
        _lastIsPushed = _isPushed;
        return false;
    }
}

boolean Encoder::wasReleased() {
    if(!_isPushed && _lastIsPushed) {
        _lastIsPushed = false;
        return true;
    }
    else {
        _lastIsPushed = _isPushed;
        return false;
    }
}

boolean Encoder::isPushed() {
    return _isPushed;
}