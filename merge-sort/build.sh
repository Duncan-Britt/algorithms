#!/bin/sh

set -xe

gcc -fsanitize=address -std=c89 -Wall -Wextra main.c -o app
