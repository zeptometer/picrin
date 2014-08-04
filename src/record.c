/**
 * See Copyright Notice in picrin.h
 */

#include "picrin.h"
#include "picrin/record.h"

struct pic_record *
pic_record_new(pic_state *pic, pic_value rectype)
{
  struct pic_record *rec;

  rec = (struct pic_record *)pic_obj_alloc(pic, sizeof(struct pic_record), PIC_TT_RECORD);
  rec->rectype = rectype;
  xh_init_int(&rec->hash, sizeof(pic_value));

  return rec;
}

bool
pic_record_of(pic_state *pic, struct pic_record *rec, pic_value rectype)
{
  UNUSED(pic);

  return pic_eq_p(rec->rectype, rectype);
}

pic_value
pic_record_ref(pic_state *pic, struct pic_record *rec, pic_sym slotname)
{
  xh_entry *e;

  e = xh_get_int(&rec->hash, slotname);
  if (! e) {
    pic_errorf(pic, "slot named ~s is not found for record: ~s", pic_sym_value(slotname), rec);
  }
  return xh_val(e, pic_value);
}


void
pic_record_set(pic_state *pic, struct pic_record *rec, pic_sym slotname, pic_value val)
{
  UNUSED(pic);

  xh_put_int(&rec->hash, slotname, &val);
}

static pic_value
pic_record_record(pic_state *pic)
{
  struct pic_record * rec;
  pic_value rectype;

  pic_get_args(pic, "o", &rectype);

  rec = pic_record_new(pic, rectype);

  return pic_obj_value(rec);
}

static pic_value
pic_record_record_of(pic_state *pic)
{
  pic_value obj, rectype;

  pic_get_args(pic, "oo", &obj, &rectype);

  return pic_bool_value(pic_record_p(obj) && pic_record_of(pic, pic_record_ptr(obj), rectype));
}

static pic_value
pic_record_record_ref(pic_state *pic)
{
  struct pic_record *rec;
  pic_sym slotname;

  pic_get_args(pic, "rm", &rec, &slotname);

  return pic_record_ref(pic, rec, slotname);
}

static pic_value
pic_record_record_set(pic_state *pic)
{
  struct pic_record *rec;
  pic_sym slotname;
  pic_value val;

  pic_get_args(pic, "rmo", &rec, &slotname, &val);

  pic_record_set(pic, rec, slotname, val);

  return pic_none_value();
}

void
pic_init_record(pic_state *pic)
{
  pic_deflibrary (pic, "(picrin record)") {
    pic_defun(pic, "make-record", pic_record_record);
    pic_defun(pic, "record-of?", pic_record_record_of);
    pic_defun(pic, "record-ref", pic_record_record_ref);
    pic_defun(pic, "record-set!", pic_record_record_set);
  }
}
