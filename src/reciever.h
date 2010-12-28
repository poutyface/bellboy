#ifndef _reciever_h_
#define _reciever_h_

int (*RecieveCallback)(int fd, void *data);

typedef struct Reciever{
  int fd;
  RecieveCallback call;
  void *data;
}Reciever;

#endif
