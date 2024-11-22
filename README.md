# Hpps24-smartnic
## Overview

Welcome to the Hpps24 SmartNIC repository! This project was developed as part of the High Performance Processors and Systems course. The primary goal was to port the SmartNIC Enso and create a packet filtering module that analyzes MAC addresses. This module is integrated into AMD's OpenNIC framework, enhancing its capabilities for network management and performance.

## How to integrate the custom plugin into open-nic-shell

1. Use Vitis_HLS 2023.2 to generate the verilog files, create a new project and add the source files inside the `hls` folder.
2. Rename `.v` files into `.sv`, add the following line to the `p2p_250mhz.sv` file:
```verilog
`include "p2p_250mhz_regslice_both.sv"
```
3. Inside the `plugin/p2p` substitute the `p2p_250mhz.sv` file with the generated one, and add the `p2p_250mhz_regslice_both.sv` file.
4. Inside the `plugin/p2p/box_250mhz` folder, substitute the `user_plugin_250mhz_inst.vh` file with the provided one.

## How to program the FPGA
This instructions are meant to be used with the script provided by OpenNIC-shell. As the program_fpga.sh script seems to fail because of the program_fpga.tcl, here are some instructions to fix it.
1. Move into open-nic-shell/script
2. Inside board_settings, edit the file of the corresponding board (au250.tcl in this example)
3. Add `set hw_device xcu250_0`, where `xcu250_0` is the device name
4. Now run program_fpga.sh
