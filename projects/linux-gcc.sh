#!/usr/bin/sh
lsb_release -a
uname -r
g++ --version
g++ -Wall -O2 ../source/main.cpp ../source/gradient.cpp ../source/mathlib.cpp ../source/oklab.cpp && ./a.out
