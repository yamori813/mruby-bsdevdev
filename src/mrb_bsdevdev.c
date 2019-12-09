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
#include "mruby/array.h"

#include <err.h>
#include <errno.h>
#include <sysexits.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "input.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct {
  int fd;
} mrb_bsdevdev_data;

static const struct mrb_data_type mrb_bsdevdev_data_type = {
  "mrb_bsdevdev_data", mrb_free,
};

static mrb_value mrb_bsdevdev_init(mrb_state *mrb, mrb_value self)
{
  mrb_bsdevdev_data *data;
  char device[32];
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

static mrb_value mrb_bsdevdev_read(mrb_state *mrb, mrb_value self)
{
  mrb_bsdevdev_data *data = DATA_PTR(self);
  mrb_value res;
  char buf[24];
  int type, code, val;

  read(data->fd, buf, sizeof(buf));

  type = (buf[16] << 8) | buf[17];
  code = (buf[18] << 8) | buf[19];
  val = (buf[20] << 24) | (buf[21] << 16) | (buf[22] << 8) | buf[23];

  res = mrb_ary_new(mrb);
  mrb_ary_push(mrb, res, mrb_fixnum_value(type));
  mrb_ary_push(mrb, res, mrb_fixnum_value(code));
  mrb_ary_push(mrb, res, mrb_fixnum_value(val));

  return res;
}

static mrb_value mrb_bsdevdev_getname(mrb_state *mrb, mrb_value self)
{
  mrb_bsdevdev_data *data = DATA_PTR(self);

  char name[128];
  int res = ioctl(data->fd, EVIOCGNAME(128), &name);

  return mrb_str_new_cstr(mrb, name);
}


static mrb_value mrb_bsdevdev_getsw(mrb_state *mrb, mrb_value self)
{
  mrb_bsdevdev_data *data = DATA_PTR(self);

  char sw[4];
  int res = ioctl(data->fd, EVIOCGSW(4), &sw);

  return mrb_fixnum_value(sw[3]);
}

void mrb_mruby_bsdevdev_gem_init(mrb_state *mrb)
{
  struct RClass *bsdevdev;
  bsdevdev = mrb_define_class(mrb, "BsdEvdev", mrb->object_class);
  mrb_define_method(mrb, bsdevdev, "initialize", mrb_bsdevdev_init, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, bsdevdev, "getname", mrb_bsdevdev_getname, MRB_ARGS_NONE());
  mrb_define_method(mrb, bsdevdev, "getsw", mrb_bsdevdev_getsw, MRB_ARGS_NONE());
  mrb_define_method(mrb, bsdevdev, "read", mrb_bsdevdev_read, MRB_ARGS_NONE());
  DONE;
}

void mrb_mruby_bsdevdev_gem_final(mrb_state *mrb)
{
}

