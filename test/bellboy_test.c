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


static int KeyBoard_recieve(int fd, void *data)
{
  char buf[256];
  memset(buf, 0, 80);
  read(fd, buf, 80);
  printf("%s %s\n", (unsigned char*)data, buf);
  return 0;
}


static int Random_recieve(int fd, void *data)
{
  char buf[4];
  memset(buf, 0, 4);
  read(fd, buf, 4);
  printf("%s 0x%08x\n", (unsigned char*)data, (unsigned int)(buf[0]+buf[1]+buf[2]+buf[3]));
  return 0;
}


int main()
{
  int rs;
  int fd;

  rs = BellBoy_create();
  check(rs == succeed, "should success to BellBoy_create");

  // Mapping KEYBOARD(STDIN)
  rs = BellBoy_map(KEYBOARD, KeyBoard_recieve, "KEYBOARD "); 
  check(rs == succeed, "should success to BellBoy_map");

  fd = open("/dev/random", O_RDONLY);
  check(rs == succeed, "should success to open");
  // Mapping /dev/random
  rs = BellBoy_map(fd, Random_recieve, "RANDOM "); // /dev/random
  check(rs == succeed, "should success to BellBoy_map");

  fd = open("/dev/urandom", O_RDONLY);
  check(rs == succeed, "should success to open");
  // Mapping /dev/urandom
  rs = BellBoy_map(fd, Random_recieve, "U-RANDOM "); // /dev/urandom
  check(rs == succeed, "should success to BellBoy_map");

  BellBoy_start();
  BellBoy_shutdown();

  close(fd);

  return succeed;

 error:
  return fail;
}
