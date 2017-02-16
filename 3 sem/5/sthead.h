#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <assert.h>
#include <signal.h>
#include <pthread.h>

#define true 1
#define false 0
#define BAD_INPUT -1
#define FAILURE -2
