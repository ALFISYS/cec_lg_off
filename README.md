# cec\_lg\_off

**cec\_lg\_off** is a hardware addon to make old LG smart TVs with WebOS be able to be turned off with command over HDMI device connected to it. 

It is a tiny device that you connect to your TV's interfaces. 

**cec\_lg\_off** monitors commands that other devices connected to your TV over HDMI, and when it detects someone sent CEC `STANDBY` command to TV, it turns off TV

### Device photos

#### Working prototype.

![Prototype](doc/img/making_ready_device.jpg)

*brown wire connected wrong hole on this photo. Is should be connected to 2nd column (black wires)*

### Making of device

All process of building this device from AliExpress modules is described in ["making  from modules" document](doc/MAKING_MODULES.md).

### Questions

#### *Do I need to connect the device to the HDMI cable break?*

No. You need to connect it to any free HDMI port on your TV. Control commands are sent over HDMI CEC line (Pin13 on HDMI cable). This line is electrically connected together through all HDMI devices that connected together. This means when some device talks using CEC to other device, everyone hear them.

#### *Can I connect USB cable to power adapter or power bank?*

No. You need to connect it to any free USB port on your TV. When device detects STANDBY command sent to TV on CEC line, it sends equal command (power off) through USB-SERIAL adapter. Also device powers from it. 

### Bill of materials (BOM) / List of materials (LOM)

Materials for building device from modules are listed in separate [List of materials (LOM) document](doc/LOM_MODULES.md).

### Building firmware

It's recommended to configure MCU to run at 16 MHz (`F_CPU = 16000000`).
If you're using **DigiSpark USB**, configuration supports 16.5 MHz CPU.
For other `F_CPU` values, please add your F_CPU to PowerShell script [helper_constants.ps1](firmware/attiny25/helper_constants.ps1) and run it to get contents for [constants.h](firmware/attiny25/src/cec_mon_constants.h).

### Other documents

Documents used while developing this device, such as MCU datasheet, HDMI specification, etc. are located under [`doc`](doc) folder. 

### CEC emulator

A tiny emulator that sends STANDBY command periodically is located as Arduino project under [`cec_emulator_firmware`](cec_emulator_firmware) folder
