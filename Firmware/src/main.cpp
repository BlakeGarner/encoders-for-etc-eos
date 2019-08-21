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

#include <Arduino.h>

#include <i2c_t3.h>

#include "hardware.h"
#include "displays.h"
#include "userInput.h"
#include "leds.h"
#include "eeprom.h"
#include "osc.h"
#include "uiMapper.h"

// Added as it is hard-coded to intellisense config file.
// If the make file adjusts this, then good to be warned so I can go
// shift the value in the c_cpp_properties.json file also.
#if ARDUINO != 10807
#warning "Arduino Internal Constant Changed."
#endif

void setup() {
    Wire.begin(
        I2C_MODE,    // Mode should be master.
        0x00,        // Only applicable if running as slave.
        I2C_PINS,    // Which I2C Pins.
        I2C_PULLUPS, // Pullups enabled/disabled.
        I2C_RATE,    // Rate I2C bus should run at.
        I2C_OP_MODE  // How library should drive the I2C bus.
    );
    Wire.setDefaultTimeout(I2C_DEFAULT_TIMEOUT);

    
    setup_userInput();

    setup_leds();

    setup_EEPROM();

    for(int i = 0; i < 5; i++) {
        setStatusLED(i % NUM_STATUS_LEDS, 0x0000FF);
        delay(400);
        setStatusLED(i % NUM_STATUS_LEDS, 0x000000);
    }

    setup_OLEDs();
    
    writeWelcomeMessage();

    setup_OSC();
    setup_mapper();

    delay(1000);

    for(int i = 0; i < NUM_KNOBS; i++) {
        writeOLED(i, "");
    }
}


// TEMP VARIABLES for LOOP.
int32_t numClicks[NUM_KNOBS];
bool isPushed[NUM_KNOBS];
bool isDirty[NUM_KNOBS] = {
    true, true, true, true, true, true, true, true, true
};

bool butLedIsOn[NUM_BUTTONS];
bool butIsDirty[NUM_BUTTONS] = {
    true, true, true, true, true, true
};
char _str_buf[24] = "";

int buttonNum = 0;

uint32_t wsColour[NUM_KNOBS];

elapsedMillis ledButtons = 0;
bool ledsOn = true;

void loop() {

    loop_OSC();

    loop_userInput();

    loop_mapper();

    if(ledButtons > 500) {
        setStatusLED(2, (ledsOn ? LED_BLACK : 0x004444));
        ledButtons = 0;
        ledsOn = !ledsOn;
    }
}
