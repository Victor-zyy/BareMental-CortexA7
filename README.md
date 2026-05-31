#   CortexA7 Bare-Mental Programming for IMX6ULL


##  1. prerequisite

* arm-none-eabi-gcc
* cmake
* make
* IMX6uLL SDK version 2.2


##  2. Usage

First of all, you have to download the NXP SDK2.2 for imx6ull , and then extrat the srcs in another dir ,finally establish the soft-link in each case directory.

### 2.1 compile

```
which arm-none-eabi-gcc
export ARMGCC_DIR=/path/
./armgcc/build_xxx.sh
```

### 2.2 for debug using JLink

Use one process or bash shell to run the command down below.
```
JLinkGDBServer -if JTAG -device MCIMX6Y2 -scriptfile embed_ddr_inin.jlinkscript

```

Use another process to run this command:

```
 arm-none-eabi-gdb ddr_debug/gpio_app.elf --nx --ix gdbinit_imx6ull_embed 
```


