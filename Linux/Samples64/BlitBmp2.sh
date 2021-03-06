#!/bin/bash
export PATH="$PATH:../Bin64"
export OBERON=.:../Lib/Sym64
export CC="gcc -I . -I../../Lib/Mod -I../Lib/Obj64 -L../Lib -s -Os -fno-exceptions -fno-asynchronous-unwind-tables -Wl,--gc-sections"

ofront+ -m ../../Samples/BlitBmp2.Mod
if [ $? != 0 ]; then exit 1; fi
$CC BlitBmp2.c -o BlitBmp2 -lOfront64 -ldl -I/usr/include/SDL2 -L/usr/lib -lSDL2 -lSDL2_image
if [ $? != 0 ]; then exit 1; fi
