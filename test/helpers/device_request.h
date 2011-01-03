#ifndef _device_request_h_
#define _device_request_h_

typedef struct DeviceRequest{
  int status;
  int controller;
  int action;
  unsigned int params[5];
}DeviceRequest;


typedef enum {
  GET = 0xB0,
  PUT
}DeviceRequestMethod;


typedef enum {
  NONE,
  SHUTDOWN,
  OPEN,
  CLOSE,
  UPDATE,
  PROCESS
}DeviceRequestAction;

#endif
