@echo off
setlocal

cd /d "%~dp0"
gcc -Wall -Wextra -Wpedantic -pthread main.c include\utils.c include\distance.c include\result.c include\thread.c -o main.exe -lm
if errorlevel 1 exit /b %errorlevel%

main.exe
if errorlevel 1 exit /b %errorlevel%

where python >nul 2>&1
if not errorlevel 1 (
    set "PYTHON=python"
) else (
    where py >nul 2>&1
    if not errorlevel 1 (
        set "PYTHON=py -3"
    ) else (
        echo Python 3 is not installed or is not available in PATH.
        exit /b 1
    )
)

if not exist ".\output" (
    mkdir ".\output"
    if errorlevel 1 exit /b %errorlevel%

)
if not exist ".venv\Scripts\python.exe" (
    %PYTHON% -m venv .venv
    if errorlevel 1 exit /b %errorlevel%
)

call .venv\Scripts\activate.bat
python -m pip install -r requirements.txt
if errorlevel 1 exit /b %errorlevel%

python scripts\plot.py
if errorlevel 1 exit /b %errorlevel%
