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


static int KeyBoard_receive(int fd, void *data)
{
  char buf[256];
  memset(buf, 0, 256);
  read(fd, buf, 256);
  printf("%s %s\n", (unsigned char*)data, buf);
  return true;
}


static void BellBoy_receive(BellBoyEvent what, void *data)
{
  static int num = 0;
  switch(what){
  case BellBoyHeartbeat:
    printf("HeartBeat %d\n", num);
    if(num++ > 5)
      BellBoy_stop();
    break;
  case BellBoyRuntimeError:
    printf("RuntimeError\n");
    break;
  }
}


int main()
{
  int rs;

  rs = BellBoy_create(BellBoy_receive, NULL);
  check(rs == succeed, "should success to BellBoy_create");

  // Mapping KEYBOARD(STDIN) -> KeyBoard_recieve
  rs = BellBoy_on(KEYBOARD, KeyBoard_receive, "KEYBOARD "); 
  check(rs == succeed, "should success to BellBoy_on");

  BellBoy_start();
  BellBoy_shutdown();

  return succeed;

 error:
  return fail;
}
