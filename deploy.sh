#!/bin/bash

if [ -d ot3-pro2017 ]
then
	echo Project is already cloned
	cd ot3-pro2017
	git checkout master
	git pull
	cp ../bench.py bench.py
else
	echo --------- Cloning project...
	git clone https://github.com/Sn0wFox/ot3-pro2017.git
	git fetch --All
	cd ot3-pro2017
	cp ../bench.py bench.py
	echo --------- Building project...
	cmake .
	make -f "Makefile"
fi

echo --------- Running tests...
python bench.py
echo --------- All done.
