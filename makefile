.PHONY: tests test

tests:
	cd tests/src && make
	tests/src/testOutput.exe tests/stats_mean-reference.txt build/stats_mean.txt
	tests/src/testOutput.exe tests/stats_best-reference.txt build/stats_best.txt

test:
	diff tests/stats_mean-reference.txt build/stats_mean.txt
