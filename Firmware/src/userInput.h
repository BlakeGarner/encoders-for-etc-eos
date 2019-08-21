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
 * Configures as required to process encoder input and button pushes.
 * Note that no loop_xxx method is required because changes by input
 * are driven by interrupt calls, not polling.
 */
void setup_userInput();

/**
 * Checks to see if any encoder has rotated or been pushed.
 * Runs appropriate method in uiMapper if so.
 */
void loop_userInput();

/**
 * Returns the total number of clicks the encoder has been turned since
 * this method was last called for this encoder number.
 * @param encoderNum: The encoder number to check.
 * @returns Total number of encoder clicks since last poll.
 */
int32_t encoderClicks(int encoderNum);
/**
 * Returns the total number of clicks the encoder has been turned.
 * @param encoderNum: The encoder number to check.
 * @Returns Total number of encoder clicks.
 */
int32_t totalEncoderClicks(int encoderNum);
/**
 * Returns if the encoder has been pushed since this method was last called
 * for this encoder number.
 * @param encoderNum: The encoder number to check.
 * @returns If the encoder has been pushed since last poll.
 */
bool wasEncoderPushed(int encoderNum);
/**
 * Returns if the encoder has been released since this method was last called
 * for this encoder number.
 * @param encoderNum: The encoder number to check.
 * @Returns If the encoder has been released since the last poll.
 */
bool wasEncoderReleased(int encoderNum);
/**
 * Returns if the encoder is currently held down.
 * @param encoderNum: The encoder number to check.
 * @Returns If the encoder is currently being held down.
 */
bool isEncoderPushed(int encoderNum);

/**
 * Returns if the button has been pushed since this method was last called
 * for this button number.
 * @param buttonNum: The button number to check.
 * @returns True if the button has been pushed since the last poll.
 */
bool wasButtonPushed(int buttonNum);

/**
 * Returns if the button has been released since this method was last called
 * for this button number.
 * @param buttonNum: The button number to check.
 * @returns True if the button has been released since the last poll.
 */
bool wasButtonReleased(int buttonNum);

/**
 * Returns if the button is currently pushed.
 * @param buttonNum: The button number to check.
 * @returns True if the button is currently being pushed.
 */
bool isButtonPushed(int buttonNum);