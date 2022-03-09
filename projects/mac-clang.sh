#!/bin/sh
sw_vers
clang++ --version
clang++ -std=c++14 -Wall -O2 ../source/main.cpp ../source/gradient.cpp ../source/mathlib.cpp ../source/oklab.cpp && ./a.out
