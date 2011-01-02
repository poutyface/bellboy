#ifndef _receiver_h_
#define _receiver_h_

typedef struct Receiver{
  int fd;
  int select_for_read;
  ReceiveCallback call;
  void *data;
}Receiver;

#endif
