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

/*
 * This class is fairly baisc - it could definately support some further
 * functionality - such as providing methods that allow you to check or set a
 * single GPIO pin, interrupt functionality, etc.
 * 
 * For now, it simply offers a list of the register values, along with 
 * read and write methods for both 8-bit and 16-bit data that take will use
 * the HAEN addressing, if configured.
 */

#include "MCP23S17.h"

MCP23S17::MCP23S17() {
    _chipSelectPin = 0;
}

MCP23S17::MCP23S17(int chipSelectPin, int hardwareAddress) {
    // TODO: Check is valid CS Pin
    _chipSelectPin = chipSelectPin;

    if(hardwareAddress >= 0x20 && hardwareAddress <= 0x27) {
        _readOpcode = (hardwareAddress << 1) | 0x01;
        _writeOpcode= (hardwareAddress << 1);
    }
    else {
        _readOpcode = 0x01;
        _writeOpcode = 0x00;
    }

    SPI.begin();

    pinMode(_chipSelectPin, OUTPUT);

    digitalWrite(_chipSelectPin, HIGH);

    delay(50);
}

uint8_t MCP23S17::read(uint8_t address) {
    SPI.beginTransaction(_mcpSettings);
    digitalWrite(_chipSelectPin, LOW);

    SPI.transfer(_readOpcode);
    SPI.transfer(address);
    uint8_t result = SPI.transfer(0);

    digitalWrite(_chipSelectPin, HIGH);
    SPI.endTransaction();

    return result;
}

uint16_t MCP23S17::read_16b(uint8_t address) {
    SPI.beginTransaction(_mcpSettings);
    digitalWrite(_chipSelectPin, LOW);

    SPI.transfer(_readOpcode);
    SPI.transfer(address);
    uint16_t result = SPI.transfer16(0);

    digitalWrite(_chipSelectPin, HIGH);
    SPI.endTransaction();

    return result;
}

void MCP23S17::write(uint8_t address, uint8_t data) {
    SPI.beginTransaction(_mcpSettings);
    digitalWrite(_chipSelectPin, LOW);

    SPI.transfer(_writeOpcode);
    SPI.transfer(address);
    SPI.transfer(data);

    digitalWrite(_chipSelectPin, HIGH);
    SPI.endTransaction();
}

void MCP23S17::write_16b(uint8_t address, uint16_t data) {
    SPI.beginTransaction(_mcpSettings);
    digitalWrite(_chipSelectPin, LOW);

    SPI.transfer(_writeOpcode);
    SPI.transfer(address);
    SPI.transfer16(data);

    digitalWrite(_chipSelectPin, HIGH);
    SPI.endTransaction();
}
