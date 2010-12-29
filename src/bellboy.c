#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "checker.h"
#include "boolean.h"
#include "reciever.h"


#define RECIEVERS_MAX (5)

typedef struct BellBoy_t{
  int stop_recieved;
  Reciever *recievers[RECIEVERS_MAX];
}BellBoy_t;


BellBoy_t *BellBoy=NULL;


static void bb_run();
static int bb_add_new_reciever(Reciever *rev);


int BellBoy_create()
{
  int i;

  BellBoy = (BellBoy_t*)malloc(sizeof(BellBoy_t));
  check(BellBoy != NULL, "AllocationError");

  BellBoy->stop_recieved = false;

  for(i=0; i<RECIEVERS_MAX; ++i)
    BellBoy->recievers[i] = NULL;

  return succeed;

 error:
  return fail;
}


void BellBoy_start()
{
  bb_run();
}

void BellBoy_stop()
{
  BellBoy->stop_recieved = true;
}


void BellBoy_shutdown()
{
  int i;

  for(i=0; i<RECIEVERS_MAX && BellBoy->recievers[i] != NULL; ++i)
    free(BellBoy->recievers[i]);

  free(BellBoy);
}


int BellBoy_map(int fd, RecieveCallback recieve, void *data)
{
  Reciever *rev = NULL;
  int rs;

  rev = (Reciever*)malloc(sizeof(Reciever));
  check(rev != NULL, "AllocationError");

  rev->fd   = fd;
  rev->call = recieve;
  rev->data = data;

  rs = bb_add_new_reciever(rev);
  check(rs == succeed, "should success to bb_add_new_reciever");

  return succeed;

 error:
  return fail;
}


static void bb_select_once()
{
  int i;
  fd_set fdreads;
  int fdmax = 0;
  int n;
  Reciever *rev;
  struct timeval tv;

  FD_ZERO(&fdreads);

  for(i=0; i<RECIEVERS_MAX && BellBoy->recievers[i] != NULL; ++i){
    rev = BellBoy->recievers[i];
    log_debug("bb_select_once:select %d:%d", i, rev->fd);

    FD_SET(rev->fd, &fdreads);

    if(fdmax < rev->fd)
      fdmax = rev->fd;
  }

  tv.tv_sec = 0;
  tv.tv_usec = 90000;

  n = select(fdmax+1, &fdreads, NULL, NULL, &tv);

  if(n > 0){
    for(i=0; i<RECIEVERS_MAX && BellBoy->recievers[i] != NULL; ++i){
      rev = BellBoy->recievers[i];

      if(FD_ISSET(rev->fd, &fdreads)){
        log_debug("bb_select_once:call %d:%d", i, rev->fd);
        rev->call(rev->fd, rev->data);
      }
    }
  }
}


static void bb_run()
{
  while(BellBoy->stop_recieved == false){
    bb_select_once();
  }
}


static int bb_add_new_reciever(Reciever *rev)
{
  int i;

  for(i=0; i<RECIEVERS_MAX && BellBoy->recievers[i] != NULL; ++i){}

  if(i < RECIEVERS_MAX){
    log_debug("bb_add_new_reciever: %d", i);
    BellBoy->recievers[i] = rev;
    return succeed;
  }
  else{
    return fail;
  }
}
