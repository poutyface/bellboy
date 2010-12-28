#include <stdio.h>
#include <stdlib.h>

#include "check.h"
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
  Reciever *rev;

  for(i=0; i<RECIEVERS_MAX && BellBoy->recievers[i] != NULL; ++i){
    rev = BellBoy->recievers[i];
    rev->call(rev->fd, rev->data);
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
    BellBoy->recievers[i] = rev;
    return succeed;
  }
  else{
    return fail;
  }
}
