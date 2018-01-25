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

1. Install [Cmake](https://cmake.org/download/) (version 10 or greater)
2. Clone the project:
	

	```shell
	git clone https://github.com/Sn0wFox/ot3-pro2017.git
	```


3. Build the project:

	```shell
	cmake .
	make -f Makefile
	```

4. Run the project:

	```shell
	pdc_evol_model
	```

## Benchmarks

Scripts are available to run our benchmark on grid5000 on the branch `g5k`.
There is also a batch script to automatically book a node, deploy and run the tests as a background task on it.
Don't hesitate to tweak them as needed.

Please note that you'll need to have a ssh config that look something like this:

```
# ------- g5k -------
# Alias for the gateway (not really needed, but convenient)
Host g5k
  User <username>
  Hostname access.grid5000.fr
  ForwardAgent no

# Automatic connection to hosts within Grid'5000, and not reachable direction on SSH tcp port 22
Host *.g5k
  User <username>
  ProxyCommand ssh g5k -W "`basename %h .g5k`:%p"
  ForwardAgent no
```

To run the weak and strong scaling benchmark from windows:

```shell
git fetch --all
git checkout g5k
git pull
g5k.bat
```

On linux, you can still do it by hand:

```shell
git fetch --all
git checkout g5k
git pull
ssh lyon.g5k "oarsub \"sleep 10d\" -p \"cluster='nova'\" -l walltime=8 -t allow_classic_ssh"
scp deploy.sh lyon.g5k:~/deploy.sh
ssh lyon.g5k "./deploy.sh"
```