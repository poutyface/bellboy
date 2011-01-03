#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "virtual_device.h"


DeviceRequest device = {
  .status = 0,
  .controller = DECODER_CONTROLLER,
  .action = OPEN,
};


static int device_request(DeviceRequest *params)
{
  params->status = 0;
  params->controller = device.controller;
  params->action = device.action;
  return 0;
}


static int device_response(DeviceRequest *params, int fd)
{

  switch(params->action){
  case OPEN:   device.action = UPDATE; break;
  case UPDATE: device.action = PROCESS;   break;
  case PROCESS:
    {
      int r = 0;
      char tmp;
      r = read(fd, &tmp, 1);
      if(r <= 0){
        device.action = CLOSE;
      }
      else{
        lseek(fd, -1, SEEK_CUR);
        device.action = PROCESS;
      }
    }
    break;
  case CLOSE:
    device.controller = MAIN_CONTROLLER;
    device.action = SHUTDOWN; 
    break;
  }

  return 0;
}


int vioctl(int fd, int request, void *params)
{
  int rs;

  switch(request){
  case GET:
    rs = device_request((DeviceRequest*)params);
    break;

  case PUT:
    rs = device_response((DeviceRequest*)params, fd);
    break;

  default:
    return -1;
  }

  return rs;
}

