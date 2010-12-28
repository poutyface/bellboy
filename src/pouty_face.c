#include <stdio.h>
#include <stdlib.h>

#include "check.h"
#include "boolean.h"
#include "reciever.h"

#define RECIEVERS_MAX (5)

typedef struct PoutyFace{
  int stop_recieved;
  Reciever *recievers[RECIEVERS_MAX]
}PoutyFace;


PoutyFace *PF=NULL;

static void pf_run();
static int pf_add_new_reciever(Reciever *rev);


int PF_create()
{
  int i;

  PF = (PoutyFace*)malloc(sizeof(PoutyFace));
  check(PF != NULL, "AllocationError");

  PF->stop_recieved = false;

  for(i=0; i<RECIEVERS; ++i)
    PF->recievers[i] = NULL;

  return succeed;

 error:
  return fail;
}


void PF_start()
{
  pf_run();
}


void PF_shutdown()
{
  int i;

  for(i=0; i<RECIEVERS_MAX && PF->recievers[i] != NULL; ++i)
    free(PF->recievers[i]);

  free(PF);
}


int PF_connect(int fd, RecieveCallback recieve, void *data)
{
  Reciever *rev = NULL;
  int rs;

  rev = (Reciever*)malloc(sizeof(Reciever));
  check(rev != NULL, "AllocationError");

  rev->fd   = fd;
  rev->call = recieve;
  rev->data = data;

  rs = pf_add_new_reciever(rev);
  check(rs == succeed, "should success to pf_add_new_reciever");

  return succeed;

 error:
  return fail;
}


static int pf_select_once()
{
  int i;
  Reciever *rev;

  for(i=0; i<RECIEVERS_MAX && PF->recievers[i] != NULL; ++i){
    rev = PF->recievers[i];
    rev->call(rev->fd, rev->data);
  }

}


static void pf_run()
{
  while(PF->stop_recieved == false){
    pf_select_once();
  }
}


static int pf_add_new_reciever(Reciever *rev)
{
  int i;

  for(i=0; i<RECIEVERS_MAX && PF->recievers[i] != NULL; ++i){}

  if(i < RECIEVERS_MAX){
    PF->recievers[i] = rev;
    return succeed;
  }
  else{
    return fail;
  }
}
