#ifndef HASH_DB_DEFINED
#define HASH_DB_DEFINED

#include <kcpolydb.h>

#include <nan/nan.h>

#include "export_to_v8.h"


class ExportHashDB: public ExportToV8<kyotocabinet::HashDB> {
 public:
  static void Init(v8::Handle<v8::Object> target, const char * exportName);
  static void setCustomPrototype(v8::Local<v8::FunctionTemplate> tpl);
  static NAN_METHOD(tune_alignment);
  static NAN_METHOD(tune_fbp);
  static NAN_METHOD(tune_options);
  static NAN_METHOD(tune_map);
  static NAN_METHOD(tune_buckets);
  static NAN_METHOD(tune_defrag);
};

#include "hash_db.cc"

#endif
