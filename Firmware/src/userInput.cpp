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

#include "userInput.h"
#include "hardware.h"
#include "MCP23S17.h"
#include "encoder.h"
#include "button.h"
#include "uiMapper.h"

MCP23S17 _mcp[NUM_MCP23S17];
Encoder _encoder[NUM_KNOBS];
Button _button[NUM_KNOBS];

void _setup_mcp23s17() {
    pinMode(MCP_INTA_PIN, INPUT_PULLUP);
    pinMode(MCP_INTB_PIN, INPUT_PULLUP);
    pinMode(MCP_INTC_PIN, INPUT_PULLUP);

    for(int i = 0; i < NUM_MCP23S17; i++) {
        _mcp[i] = MCP23S17(MCP_CS_PIN, MCP_ADDR_START + i);

        _mcp[i].write(MCP23S17_IOCONA, MCP_IOCON_CONFIG);
        _mcp[i].write_16b(MCP23S17_IODIRA, MCP_IODIR_CONFIG);
        _mcp[i].write_16b(MCP23S17_IPOLA, MCP_IPOL_CONFIG);
        _mcp[i].write_16b(MCP23S17_GPINTENA, MCP_GPINTEN_CONFIG[i]);
        _mcp[i].write_16b(MCP23S17_INTCONA, MCP_INTCON_CONFIG);
        _mcp[i].write_16b(MCP23S17_GPPUA, MCP_GPPU_CONFIG);
    }
}

void _setupEncodersAndButtons() {

    // Pull readings from MCP chips.
    uint16_t results[NUM_MCP23S17];
    for(int i = 0; i < NUM_MCP23S17; i++) {
        results[i] = _mcp[i].read_16b(MCP23S17_GPIOA);
    }
    
    // Initialise encoders.
    for(int i = 0; i < NUM_KNOBS; i++) {
        // Find the chip/pin index for this encoder.
        uint8_t chipIndex = ENCODER_OFFSETS[i] / NUM_OUTPUTS_PER_MCP23S17;
        uint8_t pinIndex = ENCODER_OFFSETS[i] % NUM_OUTPUTS_PER_MCP23S17;
        uint8_t encoderState 
            = (uint8_t) ((results[chipIndex] >> pinIndex) & 0x07);

        _encoder[i] = Encoder(encoderState);
    }

    // Initialise buttons.
    for(int i = 0; i < NUM_BUTTONS; i++) {
        // Find the chip/pin index for this button.
        uint8_t chipIndex = BUTTON_OFFSETS[i] / NUM_OUTPUTS_PER_MCP23S17;
        uint8_t pinIndex  = BUTTON_OFFSETS[i] % NUM_OUTPUTS_PER_MCP23S17;
        _button[i] = Button(((results[chipIndex] >> pinIndex) & 0x01) == 0);
    }
}

void _processMcpInterrupt(int mcpIndex) {
    uint16_t newValue = _mcp[mcpIndex].read_16b(MCP23S17_INTCAPA);
    for(int encIndex = 0; encIndex < NUM_KNOBS; encIndex++) {
        if(ENCODER_OFFSETS[encIndex] / NUM_OUTPUTS_PER_MCP23S17 == mcpIndex) {
            uint8_t pinIndex 
                = ENCODER_OFFSETS[encIndex] % NUM_OUTPUTS_PER_MCP23S17;
            _encoder[encIndex].processNewReading(
                (uint8_t) ((newValue >> pinIndex) & 0x07)
            );
        }
    }
    for(int butIndex = 0; butIndex < NUM_BUTTONS; butIndex++) {
        if(BUTTON_OFFSETS[butIndex] / NUM_OUTPUTS_PER_MCP23S17 == mcpIndex) {
            uint8_t pinIndex 
                = BUTTON_OFFSETS[butIndex] % NUM_OUTPUTS_PER_MCP23S17;
            _button[butIndex].processNewReading(
                ((newValue >> pinIndex) & 0x01) == 0
            );
        }
    }
}

void ISR_mcp23s17_a() {
    detachInterrupt(digitalPinToInterrupt(MCP_INTA_PIN));
    _processMcpInterrupt(0);
    attachInterrupt(
        digitalPinToInterrupt(MCP_INTA_PIN), 
        ISR_mcp23s17_a, 
        LOW);
}

void ISR_mcp23s17_b() {
    detachInterrupt(digitalPinToInterrupt(MCP_INTB_PIN));
    _processMcpInterrupt(1);
    attachInterrupt(
        digitalPinToInterrupt(MCP_INTB_PIN), 
        ISR_mcp23s17_b, 
        LOW);
}

void ISR_mcp23s17_c() {
    detachInterrupt(digitalPinToInterrupt(MCP_INTC_PIN));
    _processMcpInterrupt(2);
    attachInterrupt(
        digitalPinToInterrupt(MCP_INTC_PIN), 
        ISR_mcp23s17_c, 
        LOW);
}

void setup_userInput() {
    _setup_mcp23s17();

    _setupEncodersAndButtons();

    attachInterrupt(digitalPinToInterrupt(MCP_INTA_PIN), ISR_mcp23s17_a, LOW);
    attachInterrupt(digitalPinToInterrupt(MCP_INTB_PIN), ISR_mcp23s17_b, LOW);
    attachInterrupt(digitalPinToInterrupt(MCP_INTC_PIN), ISR_mcp23s17_c, LOW);
}

void loop_userInput() {
    int32_t numClicks = 0;
    for(int i = 0; i < NUM_KNOBS; i++) {
        if(_encoder[i].wasPushed()) {
            applyEncoderPush(i, true);
        }
        if(_encoder[i].wasReleased()) {
            applyEncoderPush(i, false);
        }
        numClicks = _encoder[i].numClicks();
        if(numClicks != 0) {
            applyEncoderMove(i, numClicks);
        }
    }

    for(int i = 0; i < NUM_BUTTONS; i++) {
        if(_button[i].wasPushed()) {
            applyButtonPush(i, true);
        }
        if(_button[i].wasReleased()) {
            applyButtonPush(i, false);
        }
    }
}

int32_t encoderClicks(int encoderNum) {
    if(encoderNum >= 0 && encoderNum < NUM_KNOBS) {
        return _encoder[encoderNum].numClicks();
    }
    return 0;
}

int32_t totalEncoderClicks(int encoderNum) {
    if(encoderNum >= 0 && encoderNum < NUM_KNOBS) {
        return _encoder[encoderNum].totalNumClicks();
    }
    return 0;
}

bool wasEncoderPushed(int encoderNum) {
    if(encoderNum >= 0 && encoderNum < NUM_KNOBS) {
        return _encoder[encoderNum].wasPushed();
    }
    return false;
}

bool wasEncoderReleased(int encoderNum) {
    if(encoderNum >= 0 && encoderNum < NUM_KNOBS) {
        return _encoder[encoderNum].wasReleased();
    }
    return false;
}

bool isEncoderPushed(int encoderNum) {
    if(encoderNum >= 0 && encoderNum < NUM_KNOBS) {
        return _encoder[encoderNum].isPushed();
    }
    return false;
}

bool wasButtonPushed(int buttonNum) {
    if(buttonNum >= 0 && buttonNum < NUM_BUTTONS) {
        return _button[buttonNum].wasPushed();
    }
    return false;
}

bool wasButtonReleased(int buttonNum) {
    if(buttonNum >= 0 && buttonNum < NUM_BUTTONS) {
        return _button[buttonNum].wasReleased();
    }
    return false;
}

bool isButtonPushed(int buttonNum) {
    if(buttonNum >= 0 && buttonNum < NUM_BUTTONS) {
        return _button[buttonNum].isPushed();
    }
    return false;
}