#ifndef _bellboy_h_
#define _bellboy_h_

#include "receiver.h"

typedef enum {
  BellBoyRuntimeError = 100,
  BellBoyHeartbeat
}BellBoyEvent;

typedef void (*BellBoyCallback)(BellBoyEvent what, void *data);

int BellBoy_create(BellBoyCallback cb, void *data);

void BellBoy_start();

void BellBoy_stop();

void BellBoy_shutdown();

int BellBoy_map(int fd, ReceiveCallback receive, void *data);

#define KEYBOARD (0)

#endif
