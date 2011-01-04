#ifndef _application_controller_h_
#define _application_controller_h_

typedef struct ApplicationController{
  int   (*open)(void **instance);
  void  (*close)(void *instance);
  int   (*update)(void *instance, void *params);
  int   (*process)(void *instance, int fd);
}ApplicationController;


#endif
