#ifndef EXPORT_TO_V8_DEFINED
#define EXPORT_TO_V8_DEFINED 1
#include <node/uv.h>
#include <node/node_buffer.h>
#include "argv_helpers.hh"
#include "standart_progress_checker.hh"
#include "exportvisitor.hh"
#include "exportcursor.hh"
template<class T> class ExportToV8: public node::ObjectWrap {
 public:     
  static void Init(v8::Handle<v8::Object> target, const char * exportName);
  static void  ExportTemplate(v8::Handle<v8::Object> target, v8::Handle<v8::FunctionTemplate> tpl, const char * exportName);
  static v8::Local<v8::FunctionTemplate> PrepareObjectTemlate(const char * exportName);
  static v8::Handle<v8::Value> New(const v8::Arguments & args); 
  static void setDefaultPrototype(v8::Local<v8::FunctionTemplate> tpl);
  static v8::Handle<v8::Value> open(const v8::Arguments & args);
  static v8::Handle<v8::Value> close(const v8::Arguments & args);
  static v8::Handle<v8::Value> size(const v8::Arguments & args);
  static v8::Handle<v8::Value> count(const v8::Arguments & args);
  static v8::Handle<v8::Value> path(const v8::Arguments & args);
  static v8::Handle<v8::Value> clear(const v8::Arguments & args);
  static v8::Handle<v8::Value> set(const v8::Arguments & args);
  static v8::Handle<v8::Value> add(const v8::Arguments & args);
  static v8::Handle<v8::Value> replace(const v8::Arguments & args);
  static v8::Handle<v8::Value> append(const v8::Arguments & args);
  static v8::Handle<v8::Value> increment(const v8::Arguments & args);
  static v8::Handle<v8::Value> cas(const v8::Arguments & args);
  static v8::Handle<v8::Value> remove(const v8::Arguments & args);
  static v8::Handle<v8::Value> get(const v8::Arguments & args);
  static v8::Handle<v8::Value> check(const v8::Arguments & args);
  static v8::Handle<v8::Value> seize(const v8::Arguments & args);
  static v8::Handle<v8::Value> set_bulk(const v8::Arguments & args);
  static v8::Handle<v8::Value> remove_bulk(const v8::Arguments & args);
  static v8::Handle<v8::Value> get_bulk(const v8::Arguments & args);
  static v8::Handle<v8::Value> status(const v8::Arguments & args);
  static v8::Handle<v8::Value> copy(const v8::Arguments & args);
  static v8::Handle<v8::Value> begin_transaction(const v8::Arguments & args);
  static v8::Handle<v8::Value> begin_transaction_try(const v8::Arguments & args);
  static v8::Handle<v8::Value> end_transaction(const v8::Arguments & args);
  static v8::Handle<v8::Value> error(const v8::Arguments & args);
  static v8::Handle<v8::Value> accept(const v8::Arguments & args);
  static v8::Handle<v8::Value> accept_bulk(const v8::Arguments & args);
  static v8::Handle<v8::Value> iterate(const v8::Arguments & args);
  static v8::Handle<v8::Value> cursor(const v8::Arguments & args);
 protected:
  //ExportToV8<T> objects has to be created ONLY through ObjectWrap
  //this means, that instances of class will be created only by static method ExportToV8<T>::New
  virtual ~ExportToV8();
  ExportToV8();
  T * db;
};
#include "exporttov8.cc"
#endif //EXPORT_TO_V8_DEFINED
