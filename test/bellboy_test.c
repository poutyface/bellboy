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
  int fd1, fd2;

  rs = BellBoy_create();
  check(rs == succeed, "should success to BellBoy_create");

  // Mapping KEYBOARD(STDIN) -> KeyBoard_recieve
  rs = BellBoy_map(KEYBOARD, KeyBoard_recieve, "KEYBOARD "); 
  check(rs == succeed, "should success to BellBoy_map");

  fd1 = open("/dev/random", O_RDONLY);
  check(rs == succeed, "should success to open");
  // Mapping /dev/random -> Random_recieve
  rs = BellBoy_map(fd1, Random_recieve, "RANDOM "); // /dev/random
  check(rs == succeed, "should success to BellBoy_map");

  fd2 = open("/dev/urandom", O_RDONLY);
  check(rs == succeed, "should success to open");
  // Mapping /dev/urandom -> Random_recieve
  rs = BellBoy_map(fd2, Random_recieve, "U-RANDOM "); // /dev/urandom
  check(rs == succeed, "should success to BellBoy_map");

  BellBoy_start();
  BellBoy_shutdown();

  close(fd1);
  close(fd2);

  return succeed;

 error:
  return fail;
}
