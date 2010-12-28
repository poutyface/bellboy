#ifndef _logger_h_
#define _logger_h_

#include <stdio.h>

#define log_error(M, ...) printf("ERROR " M "\n", ##__VA_ARGS__)
#define log_info(M, ...)  printf("INFO " M "\n", ##__VA_ARGS__)

#ifdef DEBUG
#define log_debug(M, ...) printf("DEBUG " M "\n", ##__VA_ARGS__)
#else
#define log_debug(M, ...)
#endif

#endif
