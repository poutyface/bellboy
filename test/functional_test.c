#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "miniunit.h"
#include "logger.h"
#include "boolean.h"
#include "bellboy.h"


void BellBoy_receive(BellBoyEvent what, void *data)
{
  int *user_data = (int *)data;

  switch(what){
  case BellBoyHeartbeat:
    *user_data = 1;
    BellBoy_stop();
    break;
  case BellBoyRuntimeError:
    break;
  }
}


int test_create()
{
  int rs;
  
  rs = BellBoy_create(NULL, NULL);
  mu_assert(rs == succeed, "should success to BellBoy_create");
  BellBoy_shutdown();

  rs = BellBoy_create(NULL, NULL);
  mu_assert(rs == succeed, "should success to BellBoy_ceatea");
  rs = BellBoy_create(NULL, NULL);
  mu_assert(rs == fail, "should fail to multi BellBoy_ceatea");
  BellBoy_shutdown();

  return succeed;

 error:
  return fail;
}


int test_start_stop()
{
  int rs;
  int num = 0;

  rs = BellBoy_create(BellBoy_receive, &num);
  mu_assert(rs == succeed, "should success to BellBoy_create");
  BellBoy_start();
  BellBoy_stop();
  mu_assert(num == 1, "num should be 1");
  BellBoy_shutdown();
  return succeed;

 error:
  return fail;
}


static int Callback(int fd, void *data)
{
  return true;
}


int test_mapping()
{
  int rs;
  int i;

  rs = BellBoy_create(NULL, NULL);
  mu_assert(rs == succeed, "should success to BellBoy_create");
  rs = BellBoy_map(0, Callback, NULL);
  mu_assert(rs == succeed, "should success to BellBoy_map");
  rs = BellBoy_map(0, NULL, NULL);
  mu_assert(rs == fail, "should fail to BellBoy_map with NULL Callback");
  BellBoy_shutdown();


  rs = BellBoy_create(NULL, NULL);
  mu_assert(rs == succeed, "should success to BellBoy_create");

  for(i=0; i<=MAPPING_FD_MAX; ++i){
    rs = BellBoy_map(0, Callback, NULL);
    if(i < MAPPING_FD_MAX){
      mu_assert(rs == succeed, "should success to BellBoy_map %d", i);
    }
    else if(i >= MAPPING_FD_MAX){
      mu_assert(rs == fail, "should fail to BellBoy_map %d", i);
    }
  }

  BellBoy_shutdown();

  return succeed;

 error:
  return fail;
}


int main()
{
  int rs;

  rs = test_create();
  mu_assert(rs == succeed, "should success test_creaete");
  rs = test_start_stop();
  mu_assert(rs == succeed, "should success test_start_stop");
  rs = test_mapping();
  mu_assert(rs == succeed, "should success test_mapping");

  log_info("\nall tests succeed");
  return succeed;

 error:
  return fail;
}
