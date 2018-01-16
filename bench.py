import subprocess
import time
import datetime
import os
import sys

PROGRAM_PATH    = "./pdc_evol_model"
MS_BUILD_PATH   = "D:/Program Files (x86)/Microsoft Visual Studio/2017/Community/MSBuild/15.0/Bin/MSBuild.exe"
NB_STEPS        = 10

tests_strong  = []
tests_weak    = []


size = 32
tests_strong.append(["original_fixed", 10000, size, 1])
for test in range(4):
    tests_strong.append(["test_" + str(test), 10000, size, 1])
for cores in range(2, 33, 2):
    tests_strong.append(["original_fixed", 10000, size, cores])
    for test in range(4):
        tests_strong.append(["test_" + str(test), 10000, size, cores])


size = 8
while size <= 32:
    tests_weak.append(["original_fixed", 10000, size, 1])
    for test in range(4):
        tests_weak.append(["test_" + str(test), 10000, size, 1])
    for cores in range(2, 33, 2):
        tests_weak.append(["original_fixed", 10000, size, cores])
        for test in range(4):
            tests_weak.append(["test_" + str(test), 10000, size, cores])
    size *= 2


report_name = ""


def main():
    #sanity checks

    global report_name
    report_name = "bench_strong" + datetime.datetime.fromtimestamp(time.time()).strftime('%Y_%m_%d_%H_%M_%S') + ".csv"
    
    for test in tests_strong:
        with open(report_name, 'a') as f:
            f.write(test[0] + ", ")
        checkout(test[0])
        build()
        do_test(test[1], test[2], test[3], test[0], report_name)

	report_name = "bench_weak" + datetime.datetime.fromtimestamp(time.time()).strftime('%Y_%m_%d_%H_%M_%S') + ".csv"

	for test in tests_weak:
		with open(report_name, 'a') as f:
		    f.write(test[0] + ", ")
		checkout(test[0])
		build()
		do_test(test[1], test[2], test[3], test[0], report_name)

def do_test(steps, cells, cores, commit, report_name):
    time_taken = 0;
    # sys.stdout.write("\n")
    for i in range(NB_STEPS):
        # sys.stdout.write("\r === Doing benchmark on commit : " + str(commit) + ", with parameters " + str(steps) + ", "+ str(cells) + ", "+ str(cores) + " : " + str(i+1) + "/" + str(NB_STEPS) + "===")
        sys.stdout.write("\r === Doing benchmark on commit : {}, with parameters {}, {}, {} : {:2}/{:2} (time so far {:06.2f}s) ===".format(commit, steps, cells, cores, i+1, NB_STEPS, time_taken))

        start = time.time()
        p = subprocess.Popen((PROGRAM_PATH, str(steps), str(cells), str(cores)),stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        p.communicate()
        p.wait()
        end = time.time()
        time_taken += (end-start)
    sys.stdout.write("\n")

    with open(report_name, 'a') as f:
        f.write(str(steps) + ", " + str(cells) + ", " + str(cores) + ", " + str(time_taken/NB_STEPS) + "\n")

def build():
    print(" === Compiling Binaries ... ===")
    if os.name == "nt":
        p = subprocess.Popen((MS_BUILD_PATH, os.path.abspath("../build/Project.sln"), "/t:pdc_evol_model:Rebuild", "/p:Configuration=Release"),stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        p.communicate()
        p.wait()
    if os.name == "posix":
        p = subprocess.Popen(("make", os.path.abspath(".")), pwd="../build")
        p.wait()

def checkout(commit):
    print(" === Checking out to commit : " + commit + " === ")
    p = subprocess.Popen(("git", "reset", "--hard"))
    p.wait()
    p = subprocess.Popen(("git", "checkout", commit))
    p.wait()




























if __name__ == "__main__":
    main()
    