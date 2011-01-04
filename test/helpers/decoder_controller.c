#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "application_controller.h"


typedef struct Decoder{
  int some_value;
}Decoder;


static int DecoderController_open(void **instance)
{
  Decoder *self;
  printf("\nDecoderController_open\n");
  self = (Decoder*)malloc(sizeof(Decoder));
  if(self == NULL)
    return -1;

  self->some_value = 200;

  *instance = (void*)self;

  return 0;
}


static void DecoderController_close(void *instance)
{
  Decoder *self = (Decoder*)instance;
  printf("DecoderController_close\n");

  if(self != NULL){
    free(self);
  }
}


static int DecoderController_update(void *instance, void *params)
{
  printf("DecoderController_update %d\n", ((Decoder*)instance)->some_value);
  return 0;
}


static int DecoderController_process(void *instance, int fd)
{
  char buf[256];
  int r;
  printf("DecoderController_process\n");
  memset(buf, 0, 255);
  r = read(fd, buf, 255);
  buf[r] = '\0';
  printf("%s\n", buf);
  return 0;
}


ApplicationController DecoderController = {
  .open     = DecoderController_open,
  .close    = DecoderController_close,
  .update   = DecoderController_update,
  .process  = DecoderController_process
};

