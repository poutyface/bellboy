#include <stdio.h>
#include <unistd.h>
#include "logger.h"
#include "check.h"
#include "boolean.h"
#include "bellboy.h"


static int recieve(int fd, void *data)
{
  char buf[1024];
  read(0, buf, 1024);
  log_info("%s", buf);
  return 0;
}


int main()
{
  int rs;

  rs = BellBoy_create();
  check(rs == succeed, "should success to PF_create");

  rs = BellBoy_map(0, recieve, NULL); // STDIN
  check(rs == succeed, "should success to PF_connect");

  BellBoy_start();
  BellBoy_shutdown();
  return succeed;

 error:
  return fail;
}
