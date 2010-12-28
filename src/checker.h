#ifndef _check_h_
#define _check_h_

#include "logger.h"

#define check(C, M, ...) if(!(C)){ log_error(M, ##__VA_ARGS__); goto error; }

#endif
