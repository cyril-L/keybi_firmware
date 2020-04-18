# SKB Keyboard firmware, security key included

This project aims to add a minimal keyboard and mouse interfaces to the original [Nitrokey Pro] firmware.

[Nitrokey Pro]: https://github.com/Nitrokey/nitrokey-pro-firmware

# Overview

The STM32F103 microcontroller is used by popular smart card related open-source projects like [Nitrokey Pro] and [Gnuk]. Using the USB and smart card interfaces leaves about 40 IOs that could be used for our keyboard matrix wiring needs.

Development is currently made on a [Nucleo-F103RB] development board, wired to a USB connector and a smart card jack (to be fitted with an OpenPGP Card 2.1). The wiring is described in the [hardware design of the Nitrokey Pro device](https://github.com/Nitrokey/nitrokey-pro-hardware). Alternatively, get any other development board equipped with a STM32F103TB and 128KB flash, or design your own keyboard PCB!

[Gnuk]: https://wiki.debian.org/GNUK
[Nucleo-F103RB]: https://www.st.com/en/evaluation-tools/nucleo-f103rb.html

# Building and Flashing

`make [VID=0x20a0] [PID=0x4108] firmware`

Parameters:
* VID: Define Vendor ID
* PID: Define Product ID

Flashing will depend on your board design. You don't need dedicated hardware for the [Nucleo-F103RB], it integrates the ST-LINK debugger/programmer. For custom boards, you will need to use a programmer supporting SWD, like the [Black Magic Probe].

The firmware is currently built to be located at address 0x08002000, you will need to flash the first 0x2000 bytes with the [dapboot] bootloader.

Find more information on the [Nitrokey Pro] repository.

[dapboot]: https://github.com/Nitrokey/dapboot
[Black Magic Probe]: https://1bitsquared.com/products/black-magic-probe