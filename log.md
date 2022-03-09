sRGB: 125.2ms
Linear: 619.2ms
	powf 481ms
OkLab: 2425.5ms
	cbrtf 1649ms (in Oklab)
	powf 515ms (from Linear<->sRGB conversions)


Ubuntu 20.04, gcc 9.3:    sRGB 123.1, Linear 433.6, OkLab 1567.2
Ubuntu 20.04, clang 10.0: sRGB 106.6, Linear 411.4, OkLab 1099.4
