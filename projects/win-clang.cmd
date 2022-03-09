set PATH=%PATH%;"C:\Program Files\LLVM\bin"
clang++ --version
clang++ -std=c++14 -Wall -O2 ../source/main.cpp ../source/gradient.cpp ../source/mathlib.cpp ../source/oklab.cpp && a.exe
pause
