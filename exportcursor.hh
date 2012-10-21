#ifndef EXPORT_CURSOR_DEFINED
class ExportCursor: public node::ObjectWrap {
 private:
  kyotocabinet::DB::Cursor * cursor_;
  static v8::Handle<v8::Value> New (const v8::Arguments & args);
  static void SetPrototype();
  static v8::Persistent<v8::FunctionTemplate> constructor_tpl_;
 public:
  ExportCursor();
  virtual ~ExportCursor();
  static v8::Handle<v8::Value> New(kyotocabinet::DB::Cursor * cursor);
  static v8::Handle<v8::Value> accept(const v8::Arguments & args);
  static v8::Handle<v8::Value> set_value(const v8::Arguments & args);
  static v8::Handle<v8::Value> remove(const v8::Arguments & args);
  static v8::Handle<v8::Value> get_key(const v8::Arguments & args);
  static v8::Handle<v8::Value> get_value(const v8::Arguments & args);
  static v8::Handle<v8::Value> get(const v8::Arguments & args);
  static v8::Handle<v8::Value> jump(const v8::Arguments & args);
  static v8::Handle<v8::Value> jump_back(const v8::Arguments & args);
  static v8::Handle<v8::Value> step(const v8::Arguments & args);
  static v8::Handle<v8::Value> step_back(const v8::Arguments & args);
};
#include "exportcursor.cc"
#define EXPORT_CURSOR_DEFINED 1
#endif //EXPORT_CURSOR_DEFINED
