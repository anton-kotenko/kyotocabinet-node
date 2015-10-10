#include <node.h>
#include "tools.h"

void ExportStashDB::setCustomPrototype(v8::Local<v8::FunctionTemplate> tpl){
  //use backward compatible method to populate prototype 
  //instead of node::SetPrototypeMetod
  Nan::SetPrototypeMethod(tpl, "tune_buckets", ExportStashDB::tune_buckets);
} 

NAN_METHOD(ExportStashDB::tune_buckets) {
  ExportStashDB * instance = node::ObjectWrap::Unwrap<ExportStashDB>(info.This());
  int64_t bnum; 
  REQUIRE_INT64(info, 0, bnum , "tune_buckets requires first argument to be 64 bit integer")  
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->tune_buckets(bnum)));
}

void ExportStashDB::Init(v8::Handle<v8::Object> target, const char * exportName){
  v8::Local<v8::FunctionTemplate> tpl = PrepareObjectTemlate(exportName);
  setCustomPrototype(tpl);
  ExportTemplate(target, tpl, exportName);  
}

