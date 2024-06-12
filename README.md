# Hpps24-smartnic

## How to integrate the custom plugin into open-nic-shell

1. Use Vitis_HLS 2023.2 to generate the verilog files, create a new project and add the source files inside the `hls` folder.
2. Rename `.v` files into `.sv`, add the following line to the `p2p_250mhz.sv` file:
```verilog
`include "p2p_250mhz_regslice_both.sv"
```
3. Inside the `plugin/p2p` substitute the `p2p_250mhz.sv` file with the generated one, and add the `p2p_250mhz_regslice_both.sv` file.
4. Inside the `plugin/p2p/box_250mhz` folder, substitute the `user_plugin_250mhz_inst.vh` file with the provided one.
