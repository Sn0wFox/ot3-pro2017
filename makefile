.PHONY: tests test

tests:
	tests/src/testOutput.exe tests/stats_mean-reference.txt build/stats_mean.txt 

test:
	diff tests/stats_mean-reference.txt build/stats_mean.txt