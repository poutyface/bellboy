#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "checker.h"
#include "boolean.h"
#include "bellboy.h"
#include "receiver.h"

#define RECEIVERS_MAX MAPPING_FD_MAX


typedef struct BellBoy_t{
  int stop_received;
  uint64_t next_heartbeat;
  uint64_t heartbeat_interval;
  BellBoyCallback cb;
  void *user_data;
  Receiver *receivers[RECEIVERS_MAX];
  struct timeval select_timeout;
}BellBoy_t;


BellBoy_t *BellBoy=NULL;


static void bb_run();
static int bb_add_new_receiver(Receiver *rev);
static uint64_t bb_get_time();
static void bb_heartbeat();
static void bb_update_next_heartbeat();


int BellBoy_create(BellBoyCallback cb, void *data)
{
  int i;

  check(BellBoy == NULL, "BellBoy have allready allocated");

  BellBoy = (BellBoy_t*)malloc(sizeof(BellBoy_t));
  check(BellBoy != NULL, "AllocationError");

  BellBoy->stop_received = false;
  BellBoy->next_heartbeat = 0;
  BellBoy->heartbeat_interval = 2000000;
  BellBoy->cb = cb;
  BellBoy->user_data = data;
  BellBoy->select_timeout.tv_sec = 0;
  BellBoy->select_timeout.tv_usec = 90000;

  for(i=0; i<RECEIVERS_MAX; ++i)
    BellBoy->receivers[i] = NULL;

  return succeed;

 error:
  return fail;
}


void BellBoy_start()
{
  bb_update_next_heartbeat();
  bb_run();
}


void BellBoy_stop()
{
  BellBoy->stop_received = true;
}


void BellBoy_shutdown()
{
  int i;

  if(BellBoy == NULL)
    return;

  for(i=0; i<RECEIVERS_MAX && BellBoy->receivers[i] != NULL; ++i)
    free(BellBoy->receivers[i]);

  free(BellBoy);
  BellBoy = NULL;
}


int BellBoy_on(int fd, ReceiveCallback receive, void *data)
{
  Receiver *rev = NULL;
  int rs;

  check(receive != NULL, "receive not must be NULL");

  rev = (Receiver*)malloc(sizeof(Receiver));
  check(rev != NULL, "AllocationError");

  rev->fd   = fd;
  rev->select_for_read = true;
  rev->call = receive;
  rev->data = data;

  rs = bb_add_new_receiver(rev);
  check(rs == succeed, "should success to bb_add_new_receiver");

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

  FD_ZERO(&fdreads);

  for(i=0; i<RECEIVERS_MAX && BellBoy->receivers[i] != NULL; ++i){
    Receiver *rev = BellBoy->receivers[i];
    log_debug("bb_select_once:select %d:%d", i, rev->fd);

    if(rev->select_for_read == false)
      continue;

    FD_SET(rev->fd, &fdreads);

    if(fdmax < rev->fd)
      fdmax = rev->fd;
  }

  n = select(fdmax+1, &fdreads, NULL, NULL, &BellBoy->select_timeout);

  if(n > 0){
    for(i=0; i<RECEIVERS_MAX && BellBoy->receivers[i] != NULL; ++i){
      Receiver *rev = BellBoy->receivers[i];

      if(FD_ISSET(rev->fd, &fdreads)){
        log_debug("bb_select_once:call %d:%d", i, rev->fd);
        rev->select_for_read = rev->call(rev->fd, rev->data);
      }
    }
  }
}


static void bb_run()
{
  while(BellBoy->stop_received == false){
    bb_heartbeat();
    bb_select_once();
  }
}


static int bb_add_new_receiver(Receiver *rev)
{
  int i;

  for(i=0; i<RECEIVERS_MAX && BellBoy->receivers[i] != NULL; ++i){}

  if(i < RECEIVERS_MAX){
    log_debug("bb_add_new_receiver: %d", i);
    BellBoy->receivers[i] = rev;
    return succeed;
  }
  else{
    return fail;
  }
}


static uint64_t bb_get_time()
{
  uint64_t current_time;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  current_time = (((uint64_t)(tv.tv_sec)) * 1000000LL) + ((uint64_t)(tv.tv_usec));
  return current_time;
}


static void bb_update_next_heartbeat()
{
  BellBoy->next_heartbeat = bb_get_time() + BellBoy->heartbeat_interval;
}


static void bb_heartbeat()
{
  if(BellBoy->cb == NULL)
    return;

  if(BellBoy->next_heartbeat <= bb_get_time()){
    log_debug("%lld, %lld", BellBoy->next_heartbeat, bb_get_time());
    BellBoy->cb(BellBoyHeartbeat, BellBoy->user_data);
    bb_update_next_heartbeat();
  }
}
