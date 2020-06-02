# Encoders for ETC Eos Firmware

This folder contains all the source and library files required to build the Encoders for [ETC Eos][etceos] firmware - useful for those who need to modify the code and build their own firmware.

If you only need to upload an existing firmware file to your hardware, see README.md in the EncoderBoard folder.

This is an independent project that is designed to work with the [ETC Eos][etceos], but is not affiliated with Electronic Theatre Controls.

- [Encoders for ETC Eos Firmware](#encoders-for-etc-eos-firmware)
  - [Installation - Basic](#installation---basic)
    - [Install Software](#install-software)
    - [Adjust Machine-Specific Filesystem Paths](#adjust-machine-specific-filesystem-paths)
    - [Open Project](#open-project)
    - [Build and Deploy](#build-and-deploy)
  - [Installation - Advanced](#installation---advanced)
    - [Tools Required](#tools-required)
    - [Adjust Filesystem Paths](#adjust-filesystem-paths)
    - [Build](#build)
  - [Contributing](#contributing)
  - [Credits](#credits)
  - [License](#license)

## Installation - Basic

The basic installation runs through the steps to get everything set up more-or-less the same as I did for my development.

You only need to follow these steps if you wish to build your own version of the firmware. Instructions to upload existing firmware is available in the [EncoderBoard folder](../EncoderBoard/README.md). To adjust the code, you're going to need some knowledge of C++.

### Install Software

You will need to install/extract the following:

- [**Arduino IDE v1.8.12**][arduino]
- [**Teensyduino v1.52**][teensyduino]
- [**Visual Studio Code**][vscode]
- [**VisualTeensy v0.9.7.1**][visualTeensy]
- [**TyTools v0.9.0**][tyTools]

### Adjust Machine-Specific Filesystem Paths

1. After installing/extracting, locate the following paths in `makefile`, and adjust to suit your installation:
   - **TARGET_NAME** - Name (without extension) for the firmware hex file.
   - **LIBS_LOCAL_BASE** - Folder containing the custom library files (just leave it as `lib`)
   - **CORE_BASE** - Folder containing the core library files (just leave it as `core`)
   - **GCC_BASE** - Folder containing the `arm-none-eabi-*` executables installed by Teensyduino (for example: `C:\PROGRA~2\Arduino\hardware\tools\arms\bin`)
   - **UPL_PJRC_B** - Folder containing the `teensy` executable (installed by Teensyduino) used for uploading code (for example: `C:\PROGRA~2\Arduino\hardware\tools`)
   - **UPL_TYCMD_B** - Folder containing the `TyCommanderC` executable supplied with TyTools.

2. In the .vscode/tasks.json file, adjust each path to `make.exe` to match where `make` is installed on your machine (wherever you extracted _VisualTeensy_).

3. For VSCode's C++ intellisense to work correctly, you will need to update the `compilerPath` entry in .vscode/c_cpp_properties.json to match the path of the compiler. This should be the path given in `GCC_BASE` above + `arm-none-eabi-gcc.exe` (for example: `C:/PROGRA~2/Arduino/hardware/tools/arm/bin/arm-none-eabi-gcc.exe`)

4. It shouldn't matter, but by all means update the following paths in .vsteensy/vsteensy.json:
   - **coreBase** - Path to your Teensy's base of core files installed by Teensyduino (for example: `"C:\\Program Files (x86)\\Arduino\\hardware\\teensy\\avr"`)
   - **compilerBase** - Path to the folder containing your `arm-none-eabi-*` executables installed by Teensyduino (for example: `"C\\Program Files (x86)\\Arduino\\hardware\\tools\\arm"`)

### Open Project

In Visual Studio Code, open the firmware folder as your workspace by clicking `File > Open Folder` and choosing the `Firmware` folder of this project. Now is the time to make any desired changes or improvements to the code.

In the extensions tab of Visual Studio Code, search for, download and install the C/C++ extension (ms-vscode.cpptools)

### Build and Deploy

CTRL+SHIFT+B opens the build/deploy options in the top of the window.

- **Build** - builds the project and writes the final file to .vsteensy\build\Firmware.hex (or whatever `TARGET_NAME` was set to in the makefile.
- **Clean** - Remove previous object and linker files - run before `Build` to force a recompile of everything.
- **Upload (teensy.exe)** - Upload the built code to your Teensy using the `teensy.exe` program. Before running Upload, place the Teensy in programming mode by pressing the on-board push-button just before selecting Upload (if your project is already assembled, a small hole has been included in the enclosure for a paperclip). See the upload instructions in the `EncoderBoard` folder's [README](../EncoderBoard/README.md) for more details on uploading a built hex file.
- **Upload (TyCommander)** - Upload the code to your Teensy using the `TyCommanderC.exe` program. This will use whatever firmware file was last generated using the `Build` command. Before running Upload, place the Teensy in programming mode by pressing the on-board push-button just before selecting Upload (if your project is already assembled, a small hole has been included in the enclosure for a paperclip). See the upload instructions in the `EncoderBoard` folder's [README](../EncoderBoard/README.md) for more details on uploading a built hex file.

## Installation - Advanced

The advanced installation assumes you have experience setting up your own toolchain and preferred IDE or text editor that you can configure to work with the code and compiler.

All code has been written using C++ and compiled using the GNU Embedded Toolchain for ARM included with Teensyduino.

### Tools Required

You will require the following to build and deploy the project:

- **make** - I used the binary included with VisualTeensy.
- **GNU Embedded Toolchain for Arm** - I used 5.4.1 included with Teensyduino v1.52.
- **Teensy.exe** and/or **TyCommanderC.exe** - Required to upload code to the Teensy using the Teensy's pre-loaded bootloder. Included with Teensyduino and TyTools respectively.

### Adjust Filesystem Paths

Adjust the paths used in the makefile to match your system paths for the above tools:

- **TARGET_NAME** - Name (without extension) of the output hex file.
- **LIBS_LOCAL_BASE** - Folder containing custom library files. As these are included as part of the project, you should probably just leave this as `lib`.
- **CORE_BASE** - Folder containing the Teensy's core library files. As these are included as part of the project, you should probably just leave this as `core`.
- **GCC_BASE** - Folder containing the GNU GCC Toolchain for ARM's compiler/linker binaries.
- **UPL_PJRC_B** - Folder containing the `teensy` program used to upload code to the Teensy (if installed).
- **UPL_TYCMD_B** - Folder containing the `TyCommanderC` program (if installed).

### Build

From within the Firmware folder, the following commands are available using make:

- `make clean`
- `make all`
- `make upload` - Uploads using the Teensy.exe program.
- `make uploadTy` - Uploads using the TyCommanderC.exe program.
Everything is built to the .vsteensy/build folder.

## Contributing

Contributions are most welcome!
Please log an issue and/or pull request for any bugs or feature changes you may wish to share.

## Credits

[ETC LightHack][lighthack] which formed the basis and initial inspiration for this project

[Arduino](https://github.com/arduino/Arduino) and [PJRC/Paul Stoffregen (Teensy)](https://github.com/PaulStoffregen/cores/tree/master/teensy3) for the core libraries used - various licenses.\
Adafruit's [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library) and [Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306) libraries to control the OLED displays - BSD license.\
[Enhanced I2C library for Teensy 3.x](https://github.com/nox771/i2c_t3) - see source code for credits and license.\
[CNMAT's OSC library](https://github.com/CNMAT/OSC) by Adrian Freed and Yotam Mann - see library folder for license.\
[SPI library](https://github.com/PaulStoffregen/SPI) - see source code for credits. LPGLv2 license.\
[WS2812 library](https://github.com/PaulStoffregen/WS2812Serial) by Paul Stoffregen - see code for license.

## License

All original source code (inside `src`) uses the Apache 2.0 license. See [LICENSE](../LICENSE) for more information.

Libraries inside `lib` and `core` have been licensed separately by their respective authors and include a mixture of BSD, MIT, public domain, LGPLv2 and other licenses. More information on each can be found in the relevant source files.

[arduino]: https://www.arduino.cc/en/Main/Software
[teensyduino]: https://www.pjrc.com/teensy/td_download.html
[vscode]: https://code.visualstudio.com/download
[visualTeensy]: https://github.com/luni64/VisualTeensy/releases
[tyTools]: https://github.com/Koromix/tytools/releases
[lighthack]: https://github.com/ETCLabs/lighthack
[etcEos]: https://www.etcconnect.com/Products/Consoles/Eos-Family/
