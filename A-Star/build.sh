#!/bin/sh

set -xe

gcc -fsanitize=address -std=c99 -Wall -Wextra main.c grid.c -o app
