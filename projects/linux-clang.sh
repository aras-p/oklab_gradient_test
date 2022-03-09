#!/usr/bin/sh
lsb_release -a
uname -r
clang++ --version
clang++ -Wall -O2 ../source/main.cpp ../source/gradient.cpp ../source/mathlib.cpp ../source/colorspace.cpp && ./a.out
