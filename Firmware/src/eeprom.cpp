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

#include "eeprom.h"
#include "hardware.h"
#include <i2c_t3.h>

void setup_EEPROM() {
    pinMode(EEPROM_WP_PIN, OUTPUT);
    digitalWrite(EEPROM_WP_PIN, HIGH);
}

void writeEEPROM(uint32_t eepromAddress, uint8_t data) {
    if(eepromAddress < EEPROM_TOTAL_SIZE) {
        // Determine which chip (chunk) to write to and adjusted address.
        uint8_t devIndex = (uint8_t) (eepromAddress / EEPROM_CHUNK_SIZE);
        uint16_t eepAddrAdj = (uint16_t) (eepromAddress % EEPROM_CHUNK_SIZE);

        // Actually write to the chip.
        digitalWrite(EEPROM_WP_PIN, LOW); // Write Enable EEPROM.
        Wire.beginTransmission(EEPROM_ADDR_START + devIndex);
        Wire.send((uint8_t) ((eepAddrAdj >> 8) & 0xFF));
        Wire.send((uint8_t) (eepAddrAdj & 0xFF));
        Wire.send(data);
        Wire.endTransmission();
        digitalWrite(EEPROM_WP_PIN, HIGH);
        delay(10);

    }
}

uint8_t readEEPROM(uint32_t eepromAddress) {
    uint8_t dataByte = 0x00;
    if(eepromAddress < EEPROM_TOTAL_SIZE) {
        // Determine which chip (chunk) to read to and adjusted address.
        uint8_t devIndex = (uint8_t) (eepromAddress / EEPROM_CHUNK_SIZE);
        uint16_t eepAddrAdj = (uint16_t) (eepromAddress % EEPROM_CHUNK_SIZE);
        uint8_t devAddr = EEPROM_ADDR_START + devIndex;

        // Actually read from the chip.
        Wire.beginTransmission(devAddr);
        Wire.send((uint8_t) ((eepAddrAdj >> 8) & 0xFF));
        Wire.send((uint8_t) (eepAddrAdj & 0xFF));
        Wire.endTransmission();

        Wire.requestFrom(devAddr, (size_t) 1);
        if(Wire.available()) {
            dataByte = Wire.receive();
        }
    }
    return dataByte;
}