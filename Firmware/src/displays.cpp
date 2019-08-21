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

#include "displays.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <i2c_t3.h>

#include "hardware.h"

Adafruit_SSD1306 _displays[NUM_KNOBS];

void _selectOLED(int index) {
    uint8_t tcaSelects[NUM_TCA9958A];
    for(int i = 0; i < NUM_TCA9958A; i++) {
        tcaSelects[i] = 0;
    }
    if(index >= 0 && index < NUM_KNOBS) {
        int selIndex = TCA_OLED_INDEXES[index];
        int tcaNum = selIndex / NUM_OUTPUTS_PER_TCA9958A;
        int selNum = selIndex % NUM_OUTPUTS_PER_TCA9958A;
        for(int i = 0; i < NUM_TCA9958A; i++) {
            if(i == tcaNum) {
                tcaSelects[i] = 1 << selNum;
            }
            else {
                tcaSelects[i] = 0;
            }
        }
    }
    for(int i = 0; i < NUM_TCA9958A; i++) {
        Wire.beginTransmission(TCA_ADDR_START + i);
        Wire.write(tcaSelects[i]);
        Wire.endTransmission();
    }
}

void setup_OLEDs() {
    for(int i = 0; i < NUM_KNOBS; i++) {
        Adafruit_SSD1306 disp(OLED_RESET_PIN);
        _displays[i] = disp;
        _selectOLED(i);
        _displays[i].begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
        _displays[i].setTextSize(2);
        _displays[i].setTextColor(WHITE);
        _displays[i].clearDisplay();
        _displays[i].setCursor(0,10);
        _displays[i].display();
    }
    _selectOLED(-1);
}

void writeOLED(int index, String input) {
    _selectOLED(index);
    _displays[index].clearDisplay();
    _displays[index].setCursor(0,0);
    _displays[index].print(input);
    _displays[index].display();
    _selectOLED(-1);
}

void writeWelcomeMessage() {
    for(int i = 0; i < NUM_KNOBS; i++) {
        writeOLED(i, WELCOME_MESSAGE[i]);
    }
}