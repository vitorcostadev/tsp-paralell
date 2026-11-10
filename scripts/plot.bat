@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
set "PROJECT_ROOT=%~dp0.."

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


if not exist "%PROJECT_ROOT%\.venv\Scripts\python.exe" (
    %PYTHON% -m venv "%PROJECT_ROOT%\.venv"
    if errorlevel 1 exit /b %errorlevel%
)

set "VENV_PYTHON=%PROJECT_ROOT%\.venv\Scripts\python.exe"

%VENV_PYTHON% -c "import matplotlib, pandas" >nul 2>&1
if errorlevel 1 (
    %VENV_PYTHON% -m pip install -r "%PROJECT_ROOT%\requirements.txt"
    if errorlevel 1 exit /b %errorlevel%
)

%VENV_PYTHON% "%SCRIPT_DIR%plot.py"
if errorlevel 1 exit /b %errorlevel%