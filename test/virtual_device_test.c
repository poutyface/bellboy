#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "logger.h"
#include "checker.h"
#include "boolean.h"
#include "bellboy.h"

#include "application.h"
#include "virtual_device.h"


static int dispatch(int fd, void *data)
{
  int rs = 0;
  ControllerStore *store;
  ApplicationController *controller;
  DeviceRequest request;

  rs = ioctl(fd, GET, &request);

  if(request.controller == MAIN_CONTROLLER && request.action == SHUTDOWN){
    BellBoy_stop();
    return false;
  }

  store = &app.controllers[request.controller];
  controller = store->controller;

  switch(request.action){

  case OPEN:
    store->instance = controller->open();
    request.status = store->instance != NULL ? succeed : fail;
    break;

  case CLOSE:
    controller->close(store->instance);
    store->instance = NULL;
    request.status = succeed;
    break;

  case UPDATE:
    rs = controller->update(store->instance, &request.params);
    request.status = rs == succeed ? succeed : fail;
    break;

  case PROCESS:
    rs = controller->process(store->instance, fd);
    request.status = rs == succeed ? succeed : fail;
    break;
  }

  ioctl(fd, PUT, &request);
  return true;
}


static void BellBoy_receive(BellBoyEvent what, void *data)
{
  static int num = 0;
  switch(what){
  case BellBoyHeartbeat:
    log_info("HeartBeat %d\n", num++);
    break;
  case BellBoyRuntimeError:
    log_info("RuntimeError\n");
    break;
  }
}


int main()
{
  int rs;
  int fd;

  rs = BellBoy_create(BellBoy_receive, NULL);
  check(rs == succeed, "should success to BellBoy_create");

  fd = open("test/fixtures/sample_data.txt", O_RDONLY);
  rs = BellBoy_on(fd, dispatch, NULL); 
  check(rs == succeed, "should success to BellBoy_on");

  BellBoy_start();

  BellBoy_shutdown();

  close(fd);
  return succeed;

 error:
  return fail;
}
