----
**Initial**

| Platform | sRGB | Linear | OkLab |
| --- | ---: | ---: | ---: |
| Windows, vs2022 | 125.2 | 619.2 | 2424.5 |
| Windows, clang 13 | 115.7 | 601.3 | 2405.5 |
| Linux, gcc 9.3 | 123.1 | 433.6 | 1567.2 |
| Linux, clang 10 | 106.6 | 411.4 | 1099.4 |
| Mac, clang 13 | 146.2 | 408.5 | 966.9 |

* Windows: Windows 10 21H2, AMD Ryzen 5950X.
* Linux: same as above, ran through WSL2 w/ Ubuntu 20.
* Mac: macOS 12.1, MBP M1 Max 16".
* Compiler options: `-O2` for gcc/clang, `Release` for VS, everything else left at defaults. 

Profiling Windows VS build:
* Linear spends 481ms inside `powf()`,
* OkLab spends 1649ms inside `cbrtf()`, and 515ms inside `powf()`.
