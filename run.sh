#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"
gcc -Wall -Wextra -Wpedantic -pthread \
    main.c include/utils.c include/distance.c include/result.c include/thread.c \
    -o main -lm
./main

if command -v python3 >/dev/null 2>&1; then
    PYTHON=python3
elif command -v python >/dev/null 2>&1; then
    PYTHON=python
else
    echo "Python 3 is not installed or is not available in PATH." >&2
    exit 1
fi

if [ ! -d ".venv" ]; then
    "$PYTHON" -m venv .venv
fi

if [ ! -d "output" ]; then
    mkdir output
fi

source .venv/bin/activate
python -m pip install -r requirements.txt
python scripts/plot.py
