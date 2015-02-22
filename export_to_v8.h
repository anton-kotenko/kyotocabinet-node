#ifndef EXPORT_TO_V8_DEFINED
#define EXPORT_TO_V8_DEFINED

#include <v8.h>

#include <node.h>

#include "node_modules/nan/nan.h"

#include "cursor.h"


template<class T> class ExportToV8: public iRefable {
 public:     
  static void Init(v8::Handle<v8::Object> target, const char * exportName);
  static void  ExportTemplate(v8::Handle<v8::Object> target, v8::Handle<v8::FunctionTemplate> tpl, const char * exportName);
  static v8::Local<v8::FunctionTemplate> PrepareObjectTemlate(const char * exportName);
  static void setDefaultPrototype(v8::Local<v8::FunctionTemplate> tpl);
  static NAN_METHOD(New);
   
  static NAN_METHOD(open);
  static NAN_METHOD(close); 
  static NAN_METHOD(size);
  static NAN_METHOD(count);
  static NAN_METHOD(path);

  static NAN_METHOD(status);
  static NAN_METHOD(error);

  static NAN_METHOD(clear);
  static NAN_METHOD(copy);

  static NAN_METHOD(get);
  static NAN_METHOD(set);
  static NAN_METHOD(add);
  static NAN_METHOD(replace);
  static NAN_METHOD(append);
  static NAN_METHOD(remove);
  static NAN_METHOD(check);
  static NAN_METHOD(increment);
  static NAN_METHOD(seize);
  static NAN_METHOD(cas);
  
  static NAN_METHOD(set_bulk);
  static NAN_METHOD(remove_bulk);
  static NAN_METHOD(get_bulk);

  static NAN_METHOD(begin_transaction);
  static NAN_METHOD(begin_transaction_try);
  static NAN_METHOD(end_transaction);

  static NAN_METHOD(cursor);

  ExportToV8();
  virtual ~ExportToV8();
 protected:
  T * db;
};

#include "export_to_v8.cc"

#endif
