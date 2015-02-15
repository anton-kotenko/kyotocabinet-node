#include <node.h>
#include "tools.h"

void ExportStashDB::setCustomPrototype(v8::Local<v8::FunctionTemplate> tpl){
  //use backward compatible method to populate prototype 
  //instead of node::SetPrototypeMetod
  NODE_SET_PROTOTYPE_METHOD(tpl, "tune_buckets", ExportStashDB::tune_buckets);
} 

NAN_METHOD(ExportStashDB::tune_buckets) {
  NanScope();
  ExportStashDB * instance = node::ObjectWrap::Unwrap<ExportStashDB>(args.This());
  int64_t bnum; 
  REQUIRE_INT64(args, 0, bnum , "tune_buckets requires first argument to be 64 bit integer")  
  NanReturnValue(NanNew<v8::Boolean>(instance->db->tune_buckets(bnum)));
}

void ExportStashDB::Init(v8::Handle<v8::Object> target, const char * exportName){
  v8::Local<v8::FunctionTemplate> tpl = PrepareObjectTemlate(exportName);
  setCustomPrototype(tpl);
  ExportTemplate(target, tpl, exportName);  
}

