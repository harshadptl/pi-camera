#ifndef CAMERA_H
#define CAMERA_H

#include <assert.h>
#include <errno.h>
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <math.h>

#include <unistd.h>

#include "config.h"

// 14 = <length of capture_> + <length of .jpeg> + 1 for the null byte
#define FNAME_LENGTH (14 + FNAME_DIGITS)

// Hackish bullcrap to get FNAME_DIGITS as a string when necessary.
#define STRING(x) #x
#define MACRO_STRING(x) STRING(x)

char *number2filename(char *name, int number);
void initialize();
void signal_setup();
void capture_still();
void capture();
void camera();

#endif /* defined CAMERA_H */
