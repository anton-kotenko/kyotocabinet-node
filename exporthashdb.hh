#ifndef EXPORT_HASH_DB_DEFINED
#define EXPORT_HASH_DB_DEFINED 1
#include "exporttov8.hh"
class ExportHashDB: public ExportToV8<kyotocabinet::HashDB> {
 public:
  static void Init(v8::Handle<v8::Object> target, const char * exportName);
  static void setCustomPrototype(v8::Local<v8::FunctionTemplate> tpl);
  static v8::Handle<v8::Value> tune_alignment(const v8::Arguments & args);
  static v8::Handle<v8::Value> tune_fbp(const v8::Arguments & args);
  static v8::Handle<v8::Value> tune_options(const v8::Arguments & args);
  static v8::Handle<v8::Value> tune_map(const v8::Arguments & args);
  static v8::Handle<v8::Value> tune_buckets(const v8::Arguments & args);
  static v8::Handle<v8::Value> tune_defrag(const v8::Arguments & args);
};
#include "exporthashdb.cc"
#endif //EXPORT_HASH_DB_DEFINED
