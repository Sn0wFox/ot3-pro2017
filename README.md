# ot3-pro2017

A school project aiming to optimize a code using parallelism.

## Setup

### Windows

1. Install [Visual Studio 2017](https://www.visualstudio.com/downloads/)
2. Install [Cmake](https://cmake.org/download/) (version 10 or greater)
3. Install [Cuda](https://developer.nvidia.com/cuda-downloads)
4. Clone the project:
	

	```shell
	git clone https://github.com/Sn0wFox/ot3-pro2017.git
	```

5. Build the project (adapt architecture if needed):

	```shell
	cd ot3-pro2017
	mkdir build
	pushd build
	cmake -G "Visual Studio 15 20147 Win64" ..
	popd
	```

6. Run it with Visual Studio 2017:

	```shell
	start build/pdc_evol_model.sln
	```

7. Compile and run using Visual Studio 2017

NOTE: it may be possible to build it without Visual Studio (using Mingw64 for instance), but it was not tested.

### Linux

TODO(bruno): detail setup


