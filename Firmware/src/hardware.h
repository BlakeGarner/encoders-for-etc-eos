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


/**********************************************
 *                 QUANTITIES                 *
 *********************************************/

// Item Quantities
static const int NUM_KNOBS       = 9;
static const int NUM_BUTTONS     = 6;
static const int NUM_WS2812      = 12;
static const int NUM_STATUS_LEDS = NUM_WS2812 - NUM_KNOBS;
static const int NUM_MCP23S17    = 3;
static const int NUM_TCA9958A    = 2;
static const int NUM_PAGE_GRP_BUTTONS = 4;

// Hardware Capabilities
static const int NUM_OUTPUTS_PER_TCA9958A = 8;
static const int NUM_OUTPUTS_PER_MCP23S17 = 16;

// Version
static const String FIRMWARE_VERSION = "1.0.0";
static const String FIRMWARE_DATE    = "30 May 2020";


/************************************************
 *                TEENSY CONFIG                 *
 ***********************************************/

// Pinouts
static const int OLED_RESET_PIN = -1; // Not used.
static const int WS2812_PIN     = 1;
static const int EEPROM_WP_PIN  = 2;
static const int BUTTON_AUX_RED_LED_PIN = 5;
static const int BUTTON_AUX_BLU_LED_PIN = 6;
static const int MCP_INTA_PIN   = 7;
static const int MCP_INTB_PIN   = 8;
static const int MCP_INTC_PIN   = 9;
static const int MCP_CS_PIN     = 10;
static const int BUTTON_PAGE_RED_LED_PIN = 20;
static const int BUTTON_PAGE_YEL_LED_PIN = 21;
static const int BUTTON_PAGE_GRN_LED_PIN = 22;
static const int BUTTON_PAGE_BLU_LED_PIN = 23;

static const int BUTTON_LED_PINS[NUM_BUTTONS] = {
    BUTTON_PAGE_RED_LED_PIN,
    BUTTON_PAGE_YEL_LED_PIN,
    BUTTON_PAGE_GRN_LED_PIN,
    BUTTON_PAGE_BLU_LED_PIN,
    BUTTON_AUX_BLU_LED_PIN,
    BUTTON_AUX_RED_LED_PIN};

// I2C Bus config for:
//  * 2x TCA9958s driving 9x OLEDs
//  * 4x EEPROMs
#define I2C_MODE     I2C_MASTER
#define I2C_PINS     I2C_PINS_18_19
#define I2C_PULLUPS  I2C_PULLUP_EXT
#define I2C_RATE     I2C_RATE_400 
#define I2C_OP_MODE  I2C_OP_MODE_DMA
// In microseconds
#define I2C_DEFAULT_TIMEOUT 10000

// SPI Bus config for:
//  * 3x MCP23S17s driving 9x Encoders + 6 push buttons.
// None required.


/************************************************
 *              TCA99548A Config                *
 ***********************************************/

#define TCA_ADDR_START 0x70

// I2C Output Bus Pinouts
static const int TCA_OLED_INDEXES[NUM_KNOBS] = {
  000, // (Octal) 0: 0,0
  001, // (Octal) 1: 0,1
  006, // (Octal) 2: 0,6
  010, // (Octal) 3: 1,0
  002, // (Octal) 4: 0,2
  004, // (Octal) 5: 0,4
  005, // (Octal) 6: 0,5
  011, // (Octal) 7: 1,1
  014  // (Octal) 8: 1,4
};


/************************************************
 *               MCP23S17 Config                *
 ***********************************************/

#define MCP23S17_BANK 0 // MCP_IOCON_CONFIG:7 must = 0.
#define MCP_ADDR_START 0x20

// Encoder & Button Pinouts
static const uint8_t ENCODER_OFFSETS[NUM_KNOBS]  
    = {12,  0, 28, 16,  8,  4, 24, 20, 44};
static const uint8_t BUTTON_OFFSETS[NUM_BUTTONS] 
    = {43, 42, 41, 40, 39, 38};

// Registers
//** IOCON REGISTER **/
// IOCON.BANK = 0: Registers in the same bank (addresses are sequential). Leave as 0 for register symbols to be correct.
// IOCON.MIRROR = 1: The INT pins are mirrored. INTA or INTB are associated with both PORTA and PORTB.
//               (0: The INT pins are not connected. INTA is associated with PORTA and INTB is associated with PORTB.)
// IOCON.SEQOP = 1: Runs in "Byte Mode". Sequential operation disabled, address pointer does not increment.
// IOCON.DISSLW = 0: Slew rate enabled (for SDA output).
// IOCON.HAEN = 1: Hardware Address Enable Bit (HAEN) enabled - Enables the MCP23S17 address pins.
// IOCON.ODR = 0: Configure the INT pin as an active driver output (INTPOL bit sets the polarity).
// IOCON.INTPOL = 0: Polarity of the INT output pin is Active-low (1 would be Active-high).
// IOCON.XXX = 0: Unimplemented (read as '0').
static const uint8_t MCP_IOCON_CONFIG = 0b01101000;

//** IODIRA/IODIRB REGISTER **//
// IODIRx.IO<7:0>: 1 = Pin is configured as an input (default). 
//                 0 = Pin is configured as an output.
static const uint16_t MCP_IODIR_CONFIG = 0xFFFF;

//** IPOLA/IPOLB REGISTER **//
// IOPOLx.IP<7:0>: 1 = GPIO register bit reflects the opposite logic state of the input pin.
//                 0 = GPIO register bit reflects the same logic state of the input pin. (Default).
static const uint16_t MCP_IPOL_CONFIG = 0xFFFF;

//** GPINTENA/GPINTENB REGISTER: Refer INTCON **//
// GPINTENx.GPINT<7:0>: 1 = Enables GPIO input pin for interrupt-on-change event.
//                      0 = Disables GPIO input pin for interrupt-on-change event (Default).
static const uint16_t MCP_GPINTEN_CONFIG[NUM_MCP23S17] 
    = { 0x7777, 0x7777, 0x7FC0 };

//** DEFVALA/DEFVALB REGISTER: Refer INTCON **//
// DEFVALx.DEF<7:0>: Sets the compare value for pins configured for interrupt-on-change from defaults.
//                   If the associated pin level is the opposite from the register bit, an interrupt occurs.
// Not defined.

//** INTCONA/INTCONB REGISTER **//
// INTCON.IOC<7:0>: 1 = Pin value is compared against the associated bit in the DEFVAL register.
//                  0 = Pin value is compared against the previous value (Default).
static const uint16_t MCP_INTCON_CONFIG = 0x0000;

//** GPPUA/GPPUB REGISTER **//
// GPPUx.PU<7:0>: 1 = 100k ohm pull-up enabled.
//               0 = Pull up disabled (Default).
static const uint16_t MCP_GPPU_CONFIG = 0xFFFF;

/************************************************
 *           External EEPROM Config             *
 ***********************************************/

// External EEPROM config:
// First 66536 bytes. Subsequent blocks at +1, +2,... +7
static const int EEPROM_ADDR_START = 0b01010000;
static const int EEPROM_CHUNK_SIZE = 65536;
static const int EEPROM_NUM_CHUNKS = 8;
static const int EEPROM_TOTAL_SIZE = EEPROM_CHUNK_SIZE*EEPROM_NUM_CHUNKS;


/************************************************
 *                 OLED Config                  *
 ***********************************************/

static const int OLED_ADDR = 0x3C;
static const int OLED_MAX_TEXT_LEN = 20;

static const String WELCOME_MESSAGE[NUM_KNOBS] = {
    "EOS\nEncoder", 
    "",
    "Version\n" + FIRMWARE_VERSION,
    FIRMWARE_DATE,
    "Designed\nby",
    "Blake\nGarner",
    "",
    "",
    ""
};

/************************************************
 *                WS2812 Config                 *
 ***********************************************/

static const int KNOB_LED_INDEXES[NUM_KNOBS] 
    = {8, 9, 10, 11, 7, 6, 5, 4, 3};

static const int STATUS_LED_INDEXES[NUM_STATUS_LEDS] 
    = {0, 1, 2};

static const float RGB_RED_GAMMA = 2.2;
static const float RGB_RED_BRI = 0.5;
static const float RGB_GRN_GAMMA = 2.2;
static const float RGB_GRN_BRI = 0.4;
static const float RGB_BLU_GAMMA = 2.2;
static const float RGB_BLU_BRI = 0.5;
static const float RGB_PWM_MAX_LEVEL = 255.0;

/************************************************
 *             Button LEDs Config               *
 ***********************************************/

static const float BUTTON_LED_PWM_FREQ = 8789.062; // For 72MHz clock.
static const int BUTTON_LED_PWM_RES = 12; // Matched to frequency above.

static const int BUTTON_PAGE_RED_INDEX = 0;
static const int BUTTON_PAGE_AMB_INDEX = 1;
static const int BUTTON_PAGE_GRN_INDEX = 2;
static const int BUTTON_PAGE_BLU_INDEX = 3;
static const int BUTTON_AUX_BLU_INDEX = 4;
static const int BUTTON_AUX_RED_INDEX = 5;

static const float BUTT_RED_GAMMA = 2.5;
static const float BUTT_RED_BRI = 1.0;
static const float BUTT_AMB_GAMMA = 2.5;
static const float BUTT_AMB_BRI = 0.6;
static const float BUTT_GRN_GAMMA = 2.5;
static const float BUTT_GRN_BRI = 0.6;
static const float BUTT_BLU_GAMMA = 2.5;
static const float BUTT_BLU_BRI = 0.4;

static const float BUTT_PWM_MAX_LEVEL 
    = ((float) (1 << BUTTON_LED_PWM_RES)) - 1.0;