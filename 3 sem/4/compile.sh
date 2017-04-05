#!/bin/bash

gcc -g2 sigcp1.c -o sigcp -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112L -std=c99 -lm
gcc -g2 sigcp2.c -o sigcp2 -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112L -std=c99 -lm
