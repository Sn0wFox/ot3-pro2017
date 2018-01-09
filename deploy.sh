#!/bin/bash

if [ -d ot3-pro2017 ]
then
	echo Project is already cloned
	cd ot3-pro2017
	git checkout master
	git pull
else
	echo --------- Cloning project...
	git clone https://github.com/Sn0wFox/ot3-pro2017.git
	cd ot3-pro2017
	echo --------- Building project...
	cmake .
	make -f "Makefile"
fi

echo --------- Running tests...
echo TODO: run the tests
echo --------- All done.
