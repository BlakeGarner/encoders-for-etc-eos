# Encoders for ETC Eos

A 9-encoder (un-official) add on to the [ETC Eos][etceos] Family of Lighting Consoles and Software, using OSC over USB.

![Encoders for ETC Eos](/images/encoders-overview.gif)

This is an independent project that is designed to work with the [ETC Eos][etceos], but not affiliated with Electronic Theatre Controls in any way.

- [Encoders for ETC Eos](#encoders-for-etc-eos)
  - [Quick Start](#quick-start)
  - [Usage](#usage)
    - [Status LEDs](#status-leds)
    - [Page Select Buttons](#page-select-buttons)
    - [Auxilary Buttons](#auxilary-buttons)
  - [Compatibility](#compatibility)
  - [Contributing](#contributing)
  - [Credits](#credits)
  - [License](#license)

## Quick Start

*To build the hardware, consult the README located in the [EncoderBoard](EncoderBoard/README.md) folder.*\
*To make changes to the firmware, consult the README located in the [Firmware](Firmware/README.md) folder.*

1. Connect the encoders to your console, PC or Mac using a USB-A to micro-USB cable.
2. Start up the ETCnomad software or Eos family console.
3. Navigate to Settings > System > Show Control > OSC in the software, and ensure OSC USB is enabled.\
   ![Enable OSC USB in Settings](/images/osc-enabled.png)
4. Reboot the console or restart the software.
5. The left-hand status indicator on the encoders should change from orange (not connected) to green (connected).
6. Select a channel in the Eos console or ETCnomad software, the encoders will update with any parameters applicable to the selected page.
7. Select the page to adjust desired parameters:\
    ![Switch Encoder Pages](/images/switch-pages.gif)
   - 1st (red) button: Intensity, focus and beam parameters
   - 2nd (amber) button: Colour parameters (press multiple times to switch between the 3 layouts).
   - 3rd (green) button: Not used - for future use.
   - 4th (blue) button: Framing shutters
8. Adjust parameters by spinning the encoder clockwise or anti-clockwise. A fine mode can be toggled on or off by pushing the encoder in.\
   ![Toggle Fine Mode](/images/toggle-fine-mode.gif)

## Usage

The Encoders consist of the following components:

- 9x Encoders each with integrated push-button, a small 10x2 display and RGB LED
- 4x Page-select buttons in red, amber, green and blue.
- 2x Auxilary buttons for future use.
- 3x Auxilary RGB LEDs for status feedback and future use.
- 1x Micro-USB socket for connection to the console, PC or Mac.
- 512kB of memory for future use, such as storing custom user configuration of page layouts.

![Encoder locations, buttons and LEDs](/images/encoders-annotated.jpg)

### Status LEDs

The three RGB LEDs on the right-hand side of the provide some basic functionality feedback:

- The left-hand LED is orange when not connected to any Eos software or console, green when connected.
- The middle LED is not used, reserved for future use.
- The right-hand LED flashes a dim cyan at 1Hz to indicate the encoder firmware is still running its main loop (for debugging purposes).

![Encoder Status LED Functions](/images/encoders-statusleds.jpg)

### Page Select Buttons

The four page select buttons allow the user to choose the group of parameters they wish to adjust:

1. The first (red) button has a single layout for intensity, focus and beam adjustments:\
   ![First button - Intensity, Focus and Beam Adjustments](/images/encoderpage-ifb.jpg)
2. The second (amber) button has three layouts for colour adjustments:
   1. CMY Layout:\
      ![Second button, page one - Cyan, Magenta, Yellow, CTO](/images/encoderpage-cmycto.jpg)
   2. RGB / x7 Layout:\
      ![Second button, page two - RGB or x7](/images/encoderpage-rgbx7.jpg)
   3. x7 Adjust Layout:\
      ![Second button, page two - x7 adjust](/images/encoderpage-x7adjust.jpg)
3. The third (green) button currently has no layout and is for future use.
4. The fourth (blue) button has one layout for framing shutter adjustments:\
   ![Fourth button - Framing Shutters](/images/encoderpage-framingshutters.jpg)

### Auxilary Buttons

The two circular buttons on the right hand side are currently not used and reserved for future use.

## Compatibility

The encoders will not work on any Win XPe-based consoles. It should work on any Win 7e-based Eos consoles, as well as any version of the ETCnomad software that supports OSC-over-USB. See the [ETC Lighthack GitHub][lighthack] page:
> Eos supports this functionality on Win7e-based consoles and ETCnomad for Windows in software version 2.6.1 and newer, and on ETCnomad for Mac in software version 2.7.0 and newer.
>
> *Support for XPe-based consoles or XP computers is not planned for this project. Many XPe-based consoles can be upgraded to Win7e to make them compatible with #lighthack.*

As the Lighthack project was intended for the Arduino Uno (and this project uses a Teensy 3.2), the USB Vendor ID and Product ID had to be spoofed so the encoder board looks like an Arduino instead of a Teensy. Eos consoles will ignore OSC messages from USB devices that do not have the Arduino Uno Vendor ID and Product ID.

## Contributing

Contributions are most welcome!
Please log an issue and/or pull request for any bugs or feature changes you may wish to share.

## Credits

[ETC LightHack][lighthack] which formed the basis and initial inspiration for this project

## License

All hardware files (inside `EncoderBoard`) and original source code (inside `Firmware/src`) uses the Apache 2.0 license. See [LICENSE](LICENSE) for more information.

Libraries inside `Firmare/lib` and `Firmware/core` have been licensed separately by their respective authors and include a mixture of BSD, MIT, public domain, LGPLv2 and other licenses. More information on each can be found in the relevant source files.

[lighthack]: https://github.com/ETCLabs/lighthack
[etceos]: https://www.etcconnect.com/Products/Consoles/Eos-Family/
