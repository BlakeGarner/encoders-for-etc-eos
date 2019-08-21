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

/**
 * Provide necessary configuration for OLEDs.
 * 
 * IMPORTANT: Ensure I2C is configured ahead of time
 *  using `Wire.begin(...);`
 *  Failure to do so will cause the code to hang.
 * 
 */
void setup_OLEDs();

/**
 * Writes text to a single OLED display.
 * 
 * Assumes setup_OLEDs() has already been run.
 */
void writeOLED(int index, String input);

/**
 * Writes a (pre-defined) welcome message for bootup/testing.
 */
void writeWelcomeMessage();
