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

class Button {
    private:
        volatile bool _isPushed;
        bool _lastIsPushed;
        bool _lastIsReleased;
        bool _getIsPushed();

    public:
        /**
         * Creates a new button, assuming a starting state of released.
         */
        Button();

        /**
         * Creates a new button, with the button in the starting state
         * as defined.
         * @param startingState: True if button currently pushed
         */
        Button(bool startingState);

        /**
         * Determines if the button's state has changed, based on the new
         * reading.
         * @param buttonState: True if button currently pushed
         */
        void processNewReading(bool buttonState);

        /**
         * Returns true if the button has been pushed since this method
         * was last called. False otherwise.
         */
        bool wasPushed();

        /**
         * Returns true if the button has been released since this method
         * was last called. False otherwise.
         */
        bool wasReleased();

        /**
         * Returns true if the button is currently pushed down. 
         * False otherwise.
         */
        bool isPushed();
};