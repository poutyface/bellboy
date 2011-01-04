#ifndef _application_h_
#define _application_h_

#include "application_controller.h"


ApplicationController MainController = {
  .open        = NULL,
  .close       = NULL,
  .update      = NULL,
  .process     = NULL
};


typedef struct ControllerStore{
  ApplicationController *controller;
  void *instance;
}ControllerStore;


typedef struct Application{
  ControllerStore controllers[5];
}Application;


#include "configration.h"

#endif
