#ifndef _configration_h_
#define _configration_h_

// If you use new ApplicationController,
// add header and app.controllers in this file 
// and assign DeviceRequestController index in device_request.h


// Add new ApplicationController header
#include "decoder_controller.h"

// Add new ApplicationController
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
