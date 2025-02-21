# PSP-Serial
Example showing how to communicate with Sony's PSP via serial (SIO)

This example shows two things:

## How to use Kubridge

SIO interactions on the PSP require kernel mode access meaning they cannot be used in user level code. This has been the main hinderence with the PSP SDK SIO sample as it was built for older PSP firmware which does not have this limitation.

This sample has a kernel library in the `kernel` folder. This exposes various functions to perform the SIO interactions which the user level EBOOT in the `user` folder imports and uses.

This is based on the fantastic sample provided by Joel16 and can be seen here in their PR into the PSP SDK here: [https://github.com/pspdev/pspsdk/pull/258](https://github.com/pspdev/pspsdk/pull/258)

## SIO Interaction

The PSP has a SIO port exposed via its headphones port (its how the media remote communicated with the PSP).

This example shows how that can be initialised and data read and written to it. The PSP 1, 2 and 3K models all have this port available. However i have only tested this on the 2K.

Those code for this is taken from two brilliant projects, the OP-Ditto project here: [https://github.com/operation-ditto](https://github.com/operation-ditto) and code by TokyoDrift. Original shared on the ACIDMODS forum [https://acidmods.com/forum/index.php/topic,34966.0.html?PHPSESSID=83a0ebb0570430cac654db79d72e66dc](https://acidmods.com/forum/index.php/topic,34966.0.html?PHPSESSID=83a0ebb0570430cac654db79d72e66dc) but later uploaded to github for preservation [https://github.com/unraze/PSXControllerToPSP](https://github.com/unraze/PSXControllerToPSP).

---

# Setup

To run this example a few things are needed:

## Prerequisites

* Setup the PSPSDK [https://pspdev.github.io/installation.html](https://pspdev.github.io/installation.html)
* Purchase an official media remote to harvest its cable (pick the right one for you model PSP)
* Purchase a logic level converter [https://learn.sparkfun.com/tutorials/bi-directional-logic-level-converter-hookup-guide/all](https://learn.sparkfun.com/tutorials/bi-directional-logic-level-converter-hookup-guide/all)
  * The PSP SIO runs at a voltage lower than 3.3v. So the converter is needed to interface with higher voltages
* A device with a serial port. An Arduino Uno, ESP32 or simple USB to Serial converter can be used.

## Hardware 

Dissassemble the PSP media remote, use a multimeter in continuity to determing which cables connect to which pins (do not trust the colours).

The pinout for the various models can be found here: [https://www.psdevwiki.com/psp/Serial_Adapter](https://www.psdevwiki.com/psp/Serial_Adapter).

Wire up accordingly:

* PSP `TX` to level translator `LV1`
* PSP `RX` to level translator `LV2`
* PSP `Digital Ground` to level translator `Ground`
* PSP `1.9v/2.5v supply (depends on model)` to level translator `LV`
* Connect ESP32/Arduino/USB -> Serial adapter `RX` to level trnaslator `HV1`
* Connect ESP32/Arduino/USB -> Serial adapter `TX` to level trnaslator `HV2`
* Connect ESP32/Arduino/USB -> Serial adapter `Ground` to level trnaslator `Ground`
* Connect ESP32/Arduino/USB -> Serial adapter `+v` to level trnaslator `HV`

### If using an Arduino UNO

You can hold the Arduino Uno in reset and use it like a simple USB to Serial converter by connecting the `Reset` pin to `Ground`

### If using an ESP32

You can flash the Sketch in the Arduino folder. This is simply a serial passtrhough to USB. Be sure to setup your Arduino environment for ESP32.

The sketch is using `Serial2` to connect to the PSP. The default `TX` pin is `GPIO17` and the `RX` is `GPIO16`.

[https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)

## Compile PSP

In the `PSP` folder run the command `make`. This will compile the PSP code.

Once done you should have an `EBOOT.PSP` in the `PSP/user` folder and a `kernel.prx` in the `PSP/kernel` folder. Copy both these to your PSP, the `.prx` need to be in the same folder allongside the `EBOOT.PBP`.

On the PSP run the `EBOOT`, connect the cable you have made to the PSP and open up a serial monitor tool of your choice on the PSP.

# Using it

Once setup and connected the default baud is `115200`. Open up your preferred serial monitor.

If you type a char, the PSP should print that char and respond back with some text.

Boom all done!