#!/bin/bash

gcc -g2 modsigcp.c -o sigcp -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112L -std=c99
