#!/bin/bash

gcc -g2 ml_bash_mod.c list.c list.h proc.c proc.h -o ml_bash -std=c99 -D_POSIX_C_SOURCE=1
