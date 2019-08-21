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

#include "hardware.h"
#include "uiMapper.h"
#include "pageConfig.h"
#include "knobConfig.h"
#include "osc.h"
#include "displays.h"
#include "knobState.h"
#include "systemState.h"
#include "leds.h"

PageConfig _page;
KnobConfig _knobConfig[NUM_KNOBS];
SystemState _systemState;
KnobState  _knobState[NUM_KNOBS];
int _nextOledToUpdate = 0;

void _setKnob(
    int knobNum, 
    uint8_t type, 
    uint32_t colour, 
    const char* label,
    const char* oscDesc
) {
    if(knobNum >= 0 && knobNum < NUM_KNOBS) {
        _knobConfig[knobNum] 
            = KnobConfig(type, colour, label, oscDesc);
        _knobState[knobNum].setLabel(_knobConfig[knobNum].getLabel());
        _knobState[knobNum].setColour(_knobConfig[knobNum].getColour());
    }
}

void setIsConnected() {
    if(!_systemState.isConnected()) {
        _systemState.setIsConnected(true);
    }
    setStatusLED(CONNECTED_STATUS_LED, LED_GREEN);
}

void setIsDisconnected() {
    if(_systemState.isConnected()) {
        _systemState.setIsConnected(false);
    }
    for(int i = 0; i < NUM_KNOBS; i++) {
        _knobState[i].setInactive(); // All knobs inactive until reconnected.
    }
    setStatusLED(CONNECTED_STATUS_LED, LED_AMBER);
}

// TODO: Use EEPROM to generate this info.
void _generateKnobs() {
    int pageIndex = _page.getPageIndex();
    int groupIndex = _page.getGroupIndex();
    if(groupIndex == 0) { // Red Page
        switch(pageIndex) {
            case 0:
            default:
                _setKnob(0, 0, LED_WHITE, "Intensity", "/intens");
                _setKnob(1, 0, LED_GREEN, "Pan", "/pan");
                _setKnob(2, 0, LED_GREEN, "Tilt", "/tilt");
                _setKnob(3, 0, LED_BLACK, "", "");
                _setKnob(4, 0, LED_BLUE, "Zoom", "/zoom");
                _setKnob(5, 0, LED_BLUE, "Edge", "/edge");
                _setKnob(6, 0, LED_BLUE, "Diffusion", "/diffusn");
                _setKnob(7, 0, LED_BLUE, "Iris", "/iris");
                _setKnob(8, 0, LED_BLACK, "", "");
            break;
        }
    }
    else if(groupIndex == 1) { // Amber page
        switch(pageIndex) {
            case 0:
                _setKnob(0, 0, LED_RED,    "Red",    "/red");
                _setKnob(1, 0, LED_AMBER,  "Amber",  "/amber");
                _setKnob(2, 0, LED_GREEN,  "Green",  "/green");
                _setKnob(3, 0, LED_CYAN,   "Cyan",   "/cyan");
                _setKnob(4, 0, LED_BLUE,   "Blue",   "/blue");
                _setKnob(5, 0, LED_PURPLE, "Indigo", "/indigo");
                _setKnob(6, 0, LED_LIME,   "Lime",   "/lime");
                _setKnob(7, 0, LED_WHITE,  "White",  "/white");
                _setKnob(8, 0, LED_PURPLE, "UV",     "/uv");
            break;

            case 1:
                _setKnob(0, 0, LED_BLACK,   "",        "");
                _setKnob(1, 0, LED_BLACK,   "",        "");
                _setKnob(2, 0, LED_BLACK,   "",        "");
                _setKnob(3, 0, LED_BLACK,   "",        "");
                _setKnob(4, 0, LED_CYAN,    "Cyan",    "/cyan");
                _setKnob(5, 0, LED_MAGENTA, "Magenta", "/magenta");
                _setKnob(6, 0, LED_YELLOW,  "Yellow",  "/yellow");
                _setKnob(7, 0, LED_CTO,     "CTO",     "/cto");
                _setKnob(8, 0, LED_CTB,     "CTB",     "/ctb");
            break;

            case 2:
            default:
                _setKnob(0, 0, LED_RED,    "Red Adj",    "/red adj");
                _setKnob(1, 0, LED_AMBER,  "Amber Adj",  "/amber adj");
                _setKnob(2, 0, LED_GREEN,  "Green Adj",  "/gre adj");
                _setKnob(3, 0, LED_CYAN,   "Cyan Adj",   "/cyan adj");
                _setKnob(4, 0, LED_BLUE,   "Blue Adj",   "/blu adj");
                _setKnob(5, 0, LED_PURPLE, "Indigo Adj", "/indigo adj");
                _setKnob(6, 0, LED_LIME,   "Lime Adj",   "/lime adj");
                _setKnob(7, 0, LED_WHITE,  "White Adj",  "/white adj");
                _setKnob(8, 0, LED_BLACK,  "",           "");
            break;
        }
    }
    else if(groupIndex == 2) { // Green page
        switch(pageIndex) {
            case 0:
            default:
                _setKnob(0, 0, LED_BLACK, "", "");
                _setKnob(1, 0, LED_BLACK, "", "");
                _setKnob(2, 0, LED_BLACK, "", "");
                _setKnob(3, 0, LED_BLACK, "", "");
                _setKnob(4, 0, LED_BLACK, "", "");
                _setKnob(5, 0, LED_BLACK, "", "");
                _setKnob(6, 0, LED_BLACK, "", "");
                _setKnob(7, 0, LED_BLACK, "", "");
                _setKnob(8, 0, LED_BLACK, "", "");
            break;
        }
    }
    else if(groupIndex == 3) { // Blue Page
        switch(pageIndex) {
            case 0:
            default:
            _setKnob(0, 0, LED_CYAN, "Angle A", "/angle a");
            _setKnob(1, 0, LED_CYAN, "Angle B", "/angle b");
            _setKnob(2, 0, LED_CYAN, "Angle C", "/angle c");
            _setKnob(3, 0, LED_CYAN, "Angle D", "/angle d");
            _setKnob(4, 0, LED_BLUE, "Thrust A", "/thrust a");
            _setKnob(5, 0, LED_BLUE, "Thrust B", "/thrust b");
            _setKnob(6, 0, LED_BLUE, "Thrust C", "/thrust c");
            _setKnob(7, 0, LED_BLUE, "Thrust D", "/thrust d");
            _setKnob(8, 0, LED_PURPLE, "Frm Assmly", "/frame assembly");
            break;
        }
    }
}

void _subscribeAll() {
    String subscribeString;
    for(int i = 0; i < NUM_KNOBS; i++) {
        if(!_knobConfig[i].isBlank()) {
            subscribeString = String("/eos/subscribe/param");
            subscribeString.append(_knobConfig[i].getOscDesc());
            issueAdditionalSubscribe(subscribeString.c_str());
        }
    }
}

// TODO: Actually generate these off the knobs.
void _unsubscribeAll() {
    // TODO.
}

void setup_mapper() {
    _systemState = SystemState();
    _systemState.setIsConnected(false);
    _systemState.setIsFineModeActive(false);
    for(int i = 0; i < NUM_KNOBS; i++) {
        _knobState[i] = KnobState();
    }
    applyPageChange(0, 0);
    setIsDisconnected();
}

void loop_mapper() {
    // Update the OLEDs (but one OLED per call, so this method doesn't
    // consume too much time at once).
    if(_knobState[_nextOledToUpdate].isDirty()) {
        writeOLED(
            _nextOledToUpdate, 
            _knobState[_nextOledToUpdate].getOledText()
        );
        // Just update the colour as well - may be redundant, but not that expensive.
        setKnobLED(_nextOledToUpdate, _knobState[_nextOledToUpdate].getColour());
    }
    _nextOledToUpdate++;
    if(_nextOledToUpdate >= NUM_KNOBS) {
        _nextOledToUpdate = 0;
    }
}

void applyPageChange(int pageIndex, int groupIndex) {
    if(pageIndex >= 0 
    && groupIndex >= 0 
    && groupIndex < NUM_PAGE_GRP_BUTTONS) {
        _unsubscribeAll();
        _page = PageConfig(0, pageIndex, groupIndex);
        _generateKnobs();
        _subscribeAll();
    }
    for(int i = 0; i < NUM_PAGE_GRP_BUTTONS; i++) {
        if(_page.getGroupIndex() == i) {
            setButtonLED(i, 100);
        }
        else {
            setButtonLED(i, 0);
        }
    }
}

uint8_t getNextPage(int groupIndex) {
    if(groupIndex == 1) { 
        return (_page.getPageIndex() < 2 ? _page.getPageIndex() + 1 : 0);
    }
    return 0;
}

void applyEncoderMove(int encoderNum, int32_t numClicksMoved) {
    String msgAddr = String(EOS_WHEEL_TOKEN);
    if(encoderNum >= 0 && encoderNum < NUM_KNOBS) {
        if(_knobState[encoderNum].getFineMode()) {
            msgAddr.append(FINE_TOKEN);
        }
        else {
            msgAddr.append(COARSE_TOKEN);
        }
    }
    else {
        msgAddr.append(COARSE_TOKEN);
    }
    if(encoderNum >= 0 && encoderNum < NUM_KNOBS 
    && _knobConfig[encoderNum].getType() >= 0) {
        msgAddr.append(_knobConfig[encoderNum].getOscDesc());
        sendOSCMessage(
            msgAddr, 
            numClicksMoved * _knobConfig[encoderNum].getCoarseClicks());
    }
}

void applyEncoderPush(int encoderNum, bool wasPushed) {
    if(wasPushed) {
        if(encoderNum >= 0 && encoderNum < NUM_KNOBS) {
            _knobState[encoderNum].setFineMode(
                !_knobState[encoderNum].getFineMode()
            );
        }
    }
}

void applyButtonPush(int buttonNum, bool wasPushed) {
    if( // Page button pushed.
        wasPushed 
        && buttonNum >= BUTTON_PAGE_RED_INDEX 
        && buttonNum <= BUTTON_PAGE_BLU_INDEX
    ) {
        int newPageIndex = getNextPage(buttonNum);
        applyPageChange(newPageIndex, buttonNum);
    }
    if(wasPushed) { // Any button pushed.
        setButtonLED(buttonNum, 255);
    }
    if( // Page Button released.
        !wasPushed 
        && buttonNum >= BUTTON_PAGE_RED_INDEX 
        && buttonNum <= BUTTON_PAGE_BLU_INDEX
    ) {
        setButtonLED(buttonNum, ((_page.getGroupIndex() == buttonNum) ? 100 : 0));
    }
    if( // Aux Button released.
        !wasPushed
        && buttonNum >= BUTTON_AUX_BLU_INDEX
        && buttonNum <= BUTTON_AUX_RED_INDEX
    ) {
        setButtonLED(buttonNum, 0);
    }
}

void applyOscParamMsg(OSCMessage& msg, int msgOffset) {
    //String expectedMsgAddr;
    //String outputText;
    for(int i = 0; i < NUM_KNOBS; i++) {
        switch(_knobConfig[i].getType()) {
            case 0:
                if(msg.match(_knobConfig[i].getOscDesc().c_str(), msgOffset) > 0) {
                    if(msg.size() > 0) {
                        if(msg.isFloat(0)) {
                            _knobState[i].setValue(msg.getOSCData(0)->getFloat());
                        }
                        else {
                            // Unknown data type. Set inactive for now.
                            _knobState[i].setInactive();
                        }
                    }
                    else {
                        // No data type attached - not valid for this fixture type.
                        _knobState[i].setInactive();
                    }

                }
                break;
            default:
                // Do nothing.
                break;
        }
    }
}
