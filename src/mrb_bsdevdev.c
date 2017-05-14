/*
** mrb_bsdevdev.c - BsdEvdev class
**
** Copyright (c) Hiroki Mori 2017
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mrb_bsdevdev.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct {
  char *str;
  int len;
  int fd;
} mrb_bsdevdev_data;

static const struct mrb_data_type mrb_bsdevdev_data_type = {
  "mrb_bsdevdev_data", mrb_free,
};

static mrb_value mrb_bsdevdev_init(mrb_state *mrb, mrb_value self)
{
  mrb_bsdevdev_data *data;
  char device[16];
  mrb_int num;

  data = (mrb_bsdevdev_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_bsdevdev_data_type;
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "i", &num);
  data = (mrb_bsdevdev_data *)mrb_malloc(mrb, sizeof(mrb_bsdevdev_data));
  snprintf(device, sizeof(device), "/dev/input/event%u", num);
  data->fd = open(device, O_RDWR);
  DATA_PTR(self) = data;

  return self;
}

static mrb_value mrb_bsdevdev_getsw(mrb_state *mrb, mrb_value self)
{
  mrb_bsdevdev_data *data = DATA_PTR(self);

  char sw[4];
  int res = ioctl(fd, EVIOCGSW(4), &sw);

  mrb_fixnum_value(sw[3]);
}


void mrb_mruby_bsdevdev_gem_init(mrb_state *mrb)
{
  struct RClass *bsdevdev;
  bsdevdev = mrb_define_class(mrb, "BsdEvdev", mrb->object_class);
  mrb_define_method(mrb, bsdevdev, "initialize", mrb_bsdevdev_init, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, bsdevdev, "hello", mrb_bsdevdev_getsw, MRB_ARGS_NONE());
  DONE;
}

void mrb_mruby_bsdevdev_gem_final(mrb_state *mrb)
{
}

