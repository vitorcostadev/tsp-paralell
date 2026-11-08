#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"
gcc -Wall -Wextra -Wpedantic -pthread \
    main.c include/utils.c include/distance.c include/result.c include/thread.c \
    -o main -lm
./main
