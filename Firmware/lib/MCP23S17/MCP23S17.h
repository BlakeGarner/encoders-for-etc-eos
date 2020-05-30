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

#include <SPI.h>

#define MCP23S17_MAX_SPI 10000000UL // 10MHz - as per MCP23S17 datasheet

#ifndef MCP23S17_BANK
#define MCP23S17_BANK 0
#endif

#if MCP23S17_BANK == 0
const static byte MCP23S17_IODIRA = 0x00;
const static byte MCP23S17_IODIRB = 0x01;
const static byte MCP23S17_IPOLA = 0x02;
const static byte MCP23S17_IPOLB = 0x03;
const static byte MCP23S17_GPINTENA = 0x04;
const static byte MCP23S17_GPINTENB = 0x05;
const static byte MCP23S17_DEFVALA = 0x06;
const static byte MCP23S17_DEFVALB = 0x07;
const static byte MCP23S17_INTCONA = 0x08;
const static byte MCP23S17_INTCONB = 0x09;
const static byte MCP23S17_IOCONA = 0x0A;
const static byte MCP23S17_IOCONB = 0x0B;
const static byte MCP23S17_GPPUA = 0x0C;
const static byte MCP23S17_GPPUB = 0x0D;
const static byte MCP23S17_INTFA = 0x0E;
const static byte MCP23S17_INTFB = 0x0F;
const static byte MCP23S17_INTCAPA = 0x10;
const static byte MCP23S17_INTCAPB = 0x11;
const static byte MCP23S17_GPIOA = 0x12;
const static byte MCP23S17_GPIOB = 0x13;
const static byte MCP23S17_OLATA = 0x14;
const static byte MCP23S17_OLATB = 0x15;

#elif MCP23S17_BANK == 1
const static byte MCP23S17_IODIRA = 0x00;
const static byte MCP23S17_IPOLA = 0x01;
const static byte MCP23S17_GPINTENA = 0x02;
const static byte MCP23S17_DEFVALA = 0x03;
const static byte MCP23S17_INTCONA = 0x04;
const static byte MCP23S17_IOCONA = 0x05;
const static byte MCP23S17_GPPUA = 0x06;
const static byte MCP23S17_INTFA = 0x07;
const static byte MCP23S17_INTCAPA = 0x08;
const static byte MCP23S17_GPIOA = 0x09;
const static byte MCP23S17_OLATA = 0x0A;
const static byte MCP23S17_IODIRB = 0x10;
const static byte MCP23S17_IPOLB = 0x11;
const static byte MCP23S17_GPINTENB = 0x12;
const static byte MCP23S17_DEFVALB = 0x13;
const static byte MCP23S17_INTCONB = 0x14;
const static byte MCP23S17_IOCONB = 0x15;
const static byte MCP23S17_GPPUB = 0x16;
const static byte MCP23S17_INTFB = 0x17;
const static byte MCP23S17_INTCAPB = 0x18;
const static byte MCP23S17_GPIOB = 0x19;
const static byte MCP23S17_OLATB = 0x1A;

#else
#error MCP23S17_BANK must be 0 or 1
#endif

// Some chips may need it to be slower. If so, put some ifdef's here to
// set a speed less than MCP23S17_MAX_SPI
#define __MCP23S17_SPI_SPEED MCP23S17_MAX_SPI

class MCP23S17 {
    private:
        int _chipSelectPin = 0;
        uint8_t _readOpcode = 0x01;
        uint8_t _writeOpcode = 0x00;

        SPISettings _mcpSettings = SPISettings(__MCP23S17_SPI_SPEED,
                                               MSBFIRST,
                                               SPI_MODE0);

    public:
        // Empty constructor will not correctly configure the CS pin.
        MCP23S17();
        MCP23S17(int chipSelectPin, int hardwareAddress = 0);

        // Read just a single byte.
        uint8_t read(uint8_t address);
        // Reads from starting address address.
        uint16_t read_16b(uint8_t address);
        // Writes just a single byte to the specified addressed.
        void write(uint8_t address, uint8_t data);
        // Writes two bytes, starting at the specified address.
        void write_16b(uint8_t address, uint16_t data);

};