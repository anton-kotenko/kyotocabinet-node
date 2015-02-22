#ifndef EXPORT_CURSOR_DEFINED
#define EXPORT_CURSOR_DEFINED

#include <string>

#include <v8.h>

#include<node.h>

#include <kcpolydb.h>

#include "node_modules/nan/nan.h"

#include "tools.h"

#define GET_CURSOR(args, dst)\
  kyotocabinet::DB::Cursor * dst = node::ObjectWrap::Unwrap<ExportCursor>(args.This())->cursor_;


class ExportCursor;
//Provide Ref/Unref methods on
//ExportToV8 template instantiations
//for ExportCursor. ExportCursor
//internally contains reference to
//database, it was created for, so
//need to explain GC not to remove
//database until all it's cursors will be dead
class iRefable: public node::ObjectWrap {
  friend class ExportCursor;
};

class ExportCursor : public node::ObjectWrap {
 private:
  kyotocabinet::DB::Cursor * cursor_;
  iRefable * parent_db;
  static NAN_METHOD(JsConstructor);
  static v8::Handle<v8::Function> Init();
  ExportCursor();
  ~ExportCursor(); 
 friend class v8::FunctionTemplate;
 public: 
  static v8::Handle<v8::Object> New (kyotocabinet::DB::Cursor * cursor, iRefable * parent_db);
  static NAN_METHOD(Set);
  static NAN_METHOD(Remove);
  static NAN_METHOD(GetKey);
  static NAN_METHOD(GetValue);
  static NAN_METHOD(Get);
  static NAN_METHOD(Jump);
  static NAN_METHOD(JumpBack);
  static NAN_METHOD(Step);
  static NAN_METHOD(StepBack);
};

#include "cursor.cc"

#endif
