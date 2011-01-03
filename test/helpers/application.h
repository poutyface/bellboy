#ifndef _application_h_
#define _application_h_

#include "application_controller.h"


ApplicationController MainController = {
  .open        = NULL,
  .close       = NULL,
  .update      = NULL,
  .process     = NULL
};


typedef struct Controller{
  ApplicationController *module;
  void *instance;
}Controller;


typedef struct Application{
  Controller controllers[5];
}Application;


#include "configration.h"

#endif
