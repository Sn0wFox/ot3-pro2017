.PHONY: tests

tests:
	tests/src/testOutput.exe tests/stats_mean-reference.txt build/stats_mean.txt 
