#!/bin/sh
cmake -DCMAKE_TOOLCHAIN_FILE="/home/zyy/repo/embed_linux_tutorial/SDK_2.2_MCIM6ULL/tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Ddr_release  .
make -j4
