#!/usr/bin/env bash

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

if command -v python3 >/dev/null 2>&1; then
    PYTHON=python3
elif command -v python >/dev/null 2>&1; then
    PYTHON=python
else
    echo "Python 3 is not installed or is not available in PATH." >&2
    exit 1
fi

if [ ! -d "$PROJECT_ROOT/.venv" ]; then
    "$PYTHON" -m venv "$PROJECT_ROOT/.venv"
fi

VENV_PYTHON="$PROJECT_ROOT/.venv/bin/python"

if ! "$VENV_PYTHON" -c "import matplotlib, pandas" >/dev/null 2>&1; then
    "$VENV_PYTHON" -m pip install -r "$PROJECT_ROOT/requirements.txt"
fi

"$VENV_PYTHON" "$SCRIPT_DIR/plot.py"

echo "Plotting completed successfully."
exit 0
