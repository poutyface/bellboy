#ifndef _virtual_device_helper_h_
#define _virtual_device_helper_h_

#include "device_request.h"

#define ioctl vioctl

int vioctl(int fd, int request, void *params);

#define MAIN_CONTROLLER        (0)
#define DECODER_CONTROLLER     (1)

#endif
