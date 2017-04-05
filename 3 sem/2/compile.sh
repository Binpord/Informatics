#!/bin/bash

gcc -g2 list.h list.c proc.h proc.c argv.h argv.c useless.c -o useless -std=c99 -D_POSIX_C_SOURCE=1
