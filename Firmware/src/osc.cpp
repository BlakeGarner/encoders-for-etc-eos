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

#include "osc.h"

#include <OSCMessage.h>
#include <OSCBoards.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <OSCMatch.h>
#include <OSCTiming.h>
#ifdef BOARD_HAS_USB_SERIAL
#include <SLIPEncodedUSBSerial.h>
SLIPEncodedUSBSerial SLIPSerial(thisBoardsSerialUSB);
#else
#include <SLIPEncodedSerial.h>
SLIPEncodedSerial SLIPSerial(Serial);
#endif

#include "uiMapper.h"

ConsoleType _connectedToConsole = ConsoleNone;
unsigned long _lastMessageRxTime = 0;
bool _timeoutPingSent = false;

//char str_buf_osc[50];
//const int STR_BUF_OSC_MAX_SIZE = 128;

void _issueEosSubscribes() {
    // Send out pre-set filters from header file.
    if(NUM_OSC_FILTERS > 0) {
        OSCMessage filter(OSC_FILTERS[0].c_str());
        for(int i = 1; i < NUM_OSC_FILTERS; i++) { // yes, I really mean int i = 1.
            filter.add(OSC_FILTERS[i].c_str());
        }
        SLIPSerial.beginPacket();
        filter.send(SLIPSerial);
        SLIPSerial.endPacket();
    }
}

void issueAdditionalSubscribe(String subscribeString) {
    OSCMessage sub(subscribeString.c_str());
    sub.add(SUBSCRIBE);
    SLIPSerial.beginPacket();
    sub.send(SLIPSerial);
    SLIPSerial.endPacket();
}

void _parseMsgFromEos(OSCMessage& msg, int initialOffset) {
    // If we don't think we're connected, reconnect and subscribe.
    if(_connectedToConsole != ConsoleEos) {
        _issueEosSubscribes();
        _connectedToConsole = ConsoleEos;
        setIsConnected();
    }

    // Check if it is an /eos/out/param/... message:
    int matchOffset = msg.match(PARAM_TOKEN.c_str(), initialOffset);
    if(matchOffset > 0) {
        applyOscParamMsg(msg, matchOffset + initialOffset);
    }
}

void parseOSCMessage(String& msg) {
    // Check to see if this is a handshake String
    if(msg.indexOf(HANDSHAKE_QUERY) != -1) {
        // Handshake string found.
        SLIPSerial.beginPacket();
        SLIPSerial.write(
            (const uint8_t*) HANDSHAKE_REPLY.c_str(), 
            (size_t) HANDSHAKE_REPLY.length()
        );
        SLIPSerial.endPacket();

        // An Eos would do nothing until subscried.
        // Let Eos know we want updates on some things.
        _issueEosSubscribes();
    }
    else {
        // Prepare the message for routing by filling an OSCMessage object
        // with our message string.
        OSCMessage oscMsg;
        oscMsg.fill(
            (uint8_t*) msg.c_str(), 
            (int) msg.length()
        );
        oscMsg.route(EOS_TOKEN.c_str(), _parseMsgFromEos);
    }
}

void sendOSCMessage(const String &address, float value) {
    OSCMessage msg(address.c_str());
    msg.add(value);
    SLIPSerial.beginPacket();
    msg.send(SLIPSerial);
    SLIPSerial.endPacket();
}

void setup_OSC() {
    SLIPSerial.begin(115200);

    // Send handshake reply:
    // This is necessary for reconnecting a device because it needs some time
    // for the serial port to open. The handshake message may have been sent
    // from teh console before #eosencoder was ready.
    SLIPSerial.beginPacket();
    SLIPSerial.write(
        (const uint8_t*) HANDSHAKE_REPLY.c_str(),
        (size_t) HANDSHAKE_REPLY.length()
    );
    SLIPSerial.endPacket();

    // If it's an Eos, request updates on some things.
    _issueEosSubscribes();
}

void loop_OSC() {

    static String curMsg;
    int size;
    
    size = SLIPSerial.available();
    if(size > 0) {
        // Fill the msg with all of the available bytes.
        while(size--) {
            curMsg += (char) (SLIPSerial.read());
        }
    }

    if(SLIPSerial.endofPacket()) {
        parseOSCMessage(curMsg);
        _lastMessageRxTime = millis();
        // We only care about the ping if we haven't heard recently.
        // Clear flag when we get any traffic
        _timeoutPingSent = false;
        curMsg = String();
    }

    if(_lastMessageRxTime > 0) {
        unsigned long diff = millis() - _lastMessageRxTime;
        // We first check if it's been too long and we need to time out.
        if(diff > TIMEOUT_AFTER_IDLE_INTERVAL) {
            _connectedToConsole = ConsoleNone;
            setIsDisconnected();
            _lastMessageRxTime = 0;
            _timeoutPingSent = false;
        }


        // Send a ping if the connection has been idle too long.
        if(!_timeoutPingSent && diff > PING_AFTER_IDLE_INTERVAL) {
            OSCMessage ping(EOS_PING_TOKEN.c_str());
            ping.add(BOX_NAME_STRING "_hi");
            SLIPSerial.beginPacket();
            ping.send(SLIPSerial);
            SLIPSerial.endPacket();
            _timeoutPingSent = true;
        }
    }
}