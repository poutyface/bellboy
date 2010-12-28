#ifndef _bellboy_h_
#define _bellboy_h_

#include "reciever.h"

int BellBoy_create();

void BellBoy_start();

void BellBoy_shutdown();

int BellBoy_map(int fd, RecieveCallback recieve, void *data);

#endif
