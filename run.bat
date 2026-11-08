@echo off
setlocal

cd /d "%~dp0"
gcc -Wall -Wextra -Wpedantic -pthread main.c include\utils.c include\distance.c include\result.c include\thread.c -o main.exe -lm
if errorlevel 1 exit /b %errorlevel%

main.exe
