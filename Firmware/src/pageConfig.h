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
 * Responsible for picking out the config for the current page
 * out of EEPROM and then applying it to 
 */

 /************ EEPROM LAYOUT: GENERAL ***************
  * Mem Addr | Field Descr    | Size (Bytes) | Notes
  * ---------------------------------------------------------------------------
  * 0-23     | Name           | 24           | Simple name to describe
  * 24-47    | Version Descr  | 24           | "1.0.0a"
  * 48       | Mode           | 1            | Modes: 0 = Test, 1 = OSC  - TODO: Move to built-in EEPROM?
  * -- SECTION: TEST CONFIG INDEX --
  * 49-51    | Test Confg Adr | 3            | The memory location of test config. An address of 0 indicates no data available.
  * -- SECTION: OSC PAGES INDEX --
  * 52-54    | R0 Config Addr | 3            | The memory location of Red page 0. An address of 0 indicates no data for that page.
  * 55-57    | R1 Config Addr | 3            | The memory location of Red page 1.
  * 73-75    | R7 Config Addr | 3            | The memory location of Red page 7.
  * 76-78    | A0 Config Addr | 3            | The memory location of Amber page 0.
  * ...
  * 100-102  | G0 Config Addr | 3            | The memory location of Green page 0.
  * ...
  * 124-126  | B0 Config Addr | 3            | The memory location of Blue page 0.
  * ...
  * 145-147  | B7 Config Addr  | 3            | The memory location of Blue page 7.
  */

 /************ EEPROM LAYOUT: TEST CONFIG ***************
  * Mem Addr | Field Descr    | Size (Bytes) | Notes
  * ---------------------------------------------------------------------------
  * Nothing yet defined.
  */


 /************ EEPROM LAYOUT: OSC PAGE ***************
  *  Note that "mem addr 0" refers to the the relative address from the page index pointer in memory space.
  * Mem Addr | Field Descr      | Size (Bytes) | Notes
  * ---------------------------------------------------------------------------
  * -- FIRST ENCODER --
  * 0        | Enc A Type       | 1            | See below for allowable types.
  * 1-3      | Enc A Colour     | 3            | 24-bit RRGGBB value.
  * 4-13     | Enc A Label      | 10           | ASCII encoded string
  * -- Data fields: Type and number defined by the Encoder Type.
  * 14       | Enc A Data 0 Len | 1            | Length of first data field for encoder A.
  * 15-xx    | Enc A Data 0     | per above    | First Data Field
  * xx+1     | Enc A Data 1 Len | 1            | Length of second data field for encoder A.
  * xx+2     | Enc A Data 2 Len | per above    | Second Data Field.
  * ...
  * xx+n     | Enc A Data n Len | 1            | Length of final data field for encoder A.
  * xx+n+1   | Enc A Data n     | per above    | Final Data field for encoder A.
  * 
  * -- SECOND ENCODER --
  * As before.
  * ...
  * -- NINTH ENCODER --
  * As before.
  * ...
  * 
  * -- BLUE BUTTON --
  *          | Button Type     | 1             | See below for allowable types.
  *          | Data 0 Len      | 1             | Length of first data field for button (if applicable)
  *          | Data 0          | per above     | First data field.
  * ... Data Len/Value repeats for as many data values as are applicable to that type.
  * 
  * -- RED BUTTON --
  * Same as blue button.
  */

/************ OSC PAGES: ENCODER TYPES ************
 *  Type 0: Wheel
 *  Data 0: String for OSC description ("pan", "tilt", etc)
 *  Data 1: int16_t for amount to increase/decrease each wheel by in course mode.
 *  Data 2: int16_t for amount to increase/decrease each wheel by in fine mode.
 *  
 *  Type 1: TBC...
 */

/*********** OSC PAGES: BUTTON TYPES *************
 *  Type 0: Fine Mode
 *  Data 0: uint8_t to define momentary (0) or latching (1) behaviour.
 */

class PageConfig {
public:
    PageConfig();
    
    PageConfig(
        uint8_t type, // Page type (0 for normal)
        uint8_t pageIndex,  // Page index (0 .. x)
        uint8_t groupIndex // Page group: Red = 0, Amb = 1,.. Blu = 3.
    );
    uint8_t getType();
    uint8_t getPageIndex();
    uint8_t getGroupIndex();
private:
    uint8_t _type;
    uint8_t _pageIndex;
    uint8_t _groupIndex;
};
