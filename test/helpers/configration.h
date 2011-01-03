#ifndef _configration_h_
#define _configration_h_

#include "decoder_controller.h"

Application app = {
  .controllers = {
    { &MainController,    NULL },
    { &DecoderController, NULL },
    { NULL,               NULL },
    { NULL,               NULL },
    { NULL,               NULL },
  },
};

#endif
