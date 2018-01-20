@echo off
SETLOCAL enabledelayedexpansion

:: Create a job with n nodes
echo Reserving 1 node on Nova (Lyon)...
ssh lyon.g5k "oarsub \"sleep 10d\" -p \"cluster='nova'\" -l walltime=4 -t allow_classic_ssh | sed -n 's/OAR_JOB_ID=\(.*\)/\1/p'" > jobid
SET /p JOBID=<jobid

echo Job ID is: %JOBID%

:: Wait for nodes available and config files to be written on g5k...
:wait
SET size=0
timeout /t 5

:: List all reserved nodes...
ssh lyon.g5k "echo $(oarstat -fj %JOBID% | sed -n 's/assigned_hostnames = \(.*\)/\1/p')" > hosts.raw
for %%i in ("hosts.raw") do set size=%%~zi
if %size% leq 1 goto wait

SET /p HOSTS=<hosts.raw
SET HOSTS=%HOSTS:+= %

IF EXIST hosts.list del /f hosts.list

echo --- Hosts are:
for %%h in (%HOSTS%) do (
	echo %%h
	echo %%h >>hosts.list
)
echo.

:: Deploy project
echo --- Deploying and running...
scp deploy.sh lyon.g5k:~/deploy.sh
scp bench.py lyon.g5k:~/bench.py
ssh lyon.g5k "./deploy.sh"