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

class Encoder {
    private:
        volatile uint8_t _encoderState;
        volatile int32_t _encoderClicks;
        volatile boolean _isPushed;

        int32_t _lastUpdatedEncoderClicks;
        boolean _lastIsPushed;

    public:
        /**
         * Creates a new encoder, rotating in an unknown direction.
         * Assumes a starting state of switch released, A/B in 0b00.
         */
        Encoder();
        /**
         * Creates a new encoder, rotating in an unknown direction.
         * Requires knowledge of the encoder's position on creation.
         * This is defined in 3LSB of startingState:
         *   <2>: Switch (0 is pushed)
         *   <1:0> A/B.
         */
        Encoder(uint8_t startingState);
        
        /**
         * Determines if an encoder has moved based on its new position.
         * Also checks for any button presses.
         */
        void processNewReading(uint8_t newPosition);

        /**
         * Returns the total number of clicks the encoder has been turned
         * since this method was last called.
         */
        int32_t numClicks();

        /**
         * Returns the total number of clicks the encoder has been turned
         * since the Encoder object was created.
         */
        int32_t totalNumClicks();

        /**
         * Returns true if the encoder has been pushed since this method was
         * last called. False otherwise.
         */
        boolean wasPushed();

        /**
         * Returns true if the encoder has been released since this method
         * was last called. False otherwise.
         */
        boolean wasReleased();

        /**
         * Returns true if the encoder is currently pushed down. 
         * False otherwise.
         */
        boolean isPushed();
};