@echo off
setlocal

cd /d "%~dp0"
gcc -Wall -Wextra -Wpedantic -pthread main.c include\distance.c -o main.exe
if errorlevel 1 exit /b %errorlevel%

main.exe
