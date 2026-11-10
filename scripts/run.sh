#!/usr/bin/env bash

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

if ! printf '#include <pthread.h>
int main(void) {
    pthread_t thread;
    return pthread_create(&thread, 0, 0, 0);
}
' | gcc -x c - -pthread -o /dev/null >/dev/null 2>&1; then
    echo "GCC cannot compile and link with pthread support. Aborting." >&2
    exit 1
fi

gcc -Wall -Wextra -Wpedantic -pthread \
    main.c include/utils.c include/distance.c include/result.c include/thread.c \
    -o main -lm


if [ ! -d "output" ]; then
    mkdir output
fi

./main