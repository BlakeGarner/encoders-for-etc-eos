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
#include <OSCMessage.h>

void setup_mapper();

void loop_mapper();

void applyPageChange(int pageIndex, int groupIndex);

uint8_t getNextPageIndex(int groupIndex);

void applyEncoderMove(int encoderNum, int32_t numClicksMoved);

void applyEncoderPush(int encoderNum, bool wasPushed);

void applyButtonPush(int buttonNum, bool wasPushed);

void applyOscParamMsg(OSCMessage& msg, int msgOffset);

void setIsConnected();

void setIsDisconnected();
