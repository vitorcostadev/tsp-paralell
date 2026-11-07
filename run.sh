#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"
gcc -Wall -Wextra -Wpedantic -pthread main.c include/distance.c -o main -lm
./main
