#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "logger.h"
#include "checker.h"
#include "boolean.h"
#include "bellboy.h"


int test_create()
{
  int rs;
  
  rs = BellBoy_create(NULL, NULL);
  check(rs == succeed, "should success to BellBoy_create");
  BellBoy_shutdown();

  rs = BellBoy_create(NULL, NULL);
  check(rs == succeed, "should success to BellBoy_ceatea");
  rs = BellBoy_create(NULL, NULL);
  check(rs == fail, "should fail to multi BellBoy_ceatea");
  BellBoy_shutdown();

  return succeed;

 error:
  return false;
}

int main()
{
  int rs;

  rs = test_create();
  check(rs == succeed, "should success test_creaete");

  log_info("\nall tests succeed");
  return succeed;

 error:
  return fail;
}
