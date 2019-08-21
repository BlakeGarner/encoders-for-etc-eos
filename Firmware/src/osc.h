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

#define SUBSCRIBE    ((int32_t) 1)
#define UNSUBSCRIBE  ((int32_t) 0)

//#define OSC_BUF_MAX_SIZE   512

const String HANDSHAKE_QUERY = "ETCOSC?";
const String HANDSHAKE_REPLY = "OK";

#define VERSION_STRING              "1.0"
#define BOX_NAME_STRING             "eosencoder"
#define PING_AFTER_IDLE_INTERVAL    2500
#define TIMEOUT_AFTER_IDLE_INTERVAL 5000

enum ConsoleType {
    ConsoleNone,
    ConsoleEos,
    ConsoleCobalt,
    ConsoleColorSource
};

const String EOS_TOKEN = "/eos";
const String COBALT_TOKEN = "/cobalt";
const String COLORSOURCE_TOKEN = "/cs";
const String PARAM_TOKEN = "/out/param";
const String EOS_PING_TOKEN = "/eos/ping";

const String EOS_WHEEL_TOKEN = "/eos/wheel";
const String COARSE_TOKEN = "/coarse";
const String FINE_TOKEN = "/fine";

static const int NUM_OSC_FILTERS = 3;
static const String OSC_FILTERS[NUM_OSC_FILTERS] = {
    String("/eos/filter/add"),
    String("/eos/out/param/*"),
    String("/eos/out/ping")
};

void setup_OSC();
void loop_OSC();
void issueAdditionalSubscribe(String subscribeString);
void parseOSCMessage(String& msg);
void sendOSCMessage(const String &address, float value);
