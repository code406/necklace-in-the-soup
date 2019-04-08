#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef TYPES_H
#define TYPES_H

typedef enum {
	ERROR = 0,
	OK = 1
} Status;

typedef enum {
	FALSE = 0,
	TRUE = 1
} Bool;

#endif
/* TYPES_H */
