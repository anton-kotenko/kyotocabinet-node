#ifndef STASH_DB_DEFINED
#define STASH_DB_DEFINED

#include <v8.h>

#include <kcpolydb.h>

#include <nan.h>

#include "export_to_v8.h"


class ExportStashDB: public ExportToV8<kyotocabinet::StashDB> {
 public:
  static void Init(v8::Handle<v8::Object> target, const char * exportName);
  static void setCustomPrototype(v8::Local<v8::FunctionTemplate> tpl);
  static NAN_METHOD(tune_buckets);
};

#include "stash_db.cc"

#endif
