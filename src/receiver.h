#ifndef _receiver_h_
#define _receiver_h_

typedef int (*ReceiveCallback)(int fd, void *data);

typedef struct Receiver{
  int fd;
  ReceiveCallback call;
  void *data;
}Receiver;

#endif
