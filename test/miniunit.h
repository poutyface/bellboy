#ifndef _miniunit_h_
#define _miniunit_h_

#include <stdio.h>

#define mu_assert(C, M, ...) if(!(C)){ printf("ASSERT ERROR: %s:%d " M "\n", __FILE__, __LINE__, ##__VA_ARGS__); goto error; }

#endif

