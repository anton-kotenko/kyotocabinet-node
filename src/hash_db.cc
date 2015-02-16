#include <node.h>

#include <v8.h>

#include <kcpolydb.h>

#include "tools.h"

void ExportHashDB::Init(v8::Handle<v8::Object> target, const char * exportName){
  v8::Local<v8::FunctionTemplate> tpl = PrepareObjectTemlate(exportName);
  setCustomPrototype(tpl);
  ExportTemplate(target, tpl, exportName);  
}
void ExportHashDB::setCustomPrototype(v8::Local<v8::FunctionTemplate> tpl){
  //use backward compatible method to populate prototype 
  //instead of node::SetPrototypeMetod
  NODE_SET_PROTOTYPE_METHOD(tpl, "tune_alignment", ExportHashDB::tune_alignment);
  NODE_SET_PROTOTYPE_METHOD(tpl, "tune_fbp", ExportHashDB::tune_fbp);
  NODE_SET_PROTOTYPE_METHOD(tpl, "tune_options", ExportHashDB::tune_options);
  NODE_SET_PROTOTYPE_METHOD(tpl, "tune_buckets", ExportHashDB::tune_buckets);
  NODE_SET_PROTOTYPE_METHOD(tpl, "tune_map", ExportHashDB::tune_map);
  NODE_SET_PROTOTYPE_METHOD(tpl, "tune_defrag", ExportHashDB::tune_defrag);
  v8::Local<v8::Object> Option = NanNew<v8::Object>();
  Option->Set(NanNew<v8::String>("TSMALL"), NanNew<v8::Int32>(kyotocabinet::HashDB::TSMALL));
  Option->Set(NanNew<v8::String>("TLINEAR"), NanNew<v8::Int32>(kyotocabinet::HashDB::TLINEAR));
  Option->Set(NanNew<v8::String>("TCOMPRESS"), NanNew<v8::Int32>(kyotocabinet::HashDB::TCOMPRESS));
  tpl->PrototypeTemplate()->Set(NanNew<v8::String>("Option"), Option);
} 
NAN_METHOD(ExportHashDB::tune_alignment) {
  NanScope();
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(args.This());
  int8_t apow; 
  REQUIRE_INT8(args, 0, apow, "tune_alignment requires first argument to be 8 bit integer")  
  NanReturnValue(NanNew<v8::Boolean>(instance->db->tune_alignment(apow)));
}
NAN_METHOD(ExportHashDB::tune_fbp) {
  NanScope();
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(args.This());
  int8_t fpow; 
  REQUIRE_INT8(args, 0, fpow, "tune_fbp requires first argument to be 8 bit integer")  
  NanReturnValue(NanNew<v8::Boolean>(instance->db->tune_fbp(fpow)));
}
NAN_METHOD(ExportHashDB::tune_options) {
  NanScope();
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(args.This());
  int8_t opts; 
  REQUIRE_INT8(args, 0, opts, "tune_options requires first argument to be 8 bit integer")  
  NanReturnValue(NanNew<v8::Boolean>(instance->db->tune_options(opts)));
}
NAN_METHOD(ExportHashDB::tune_buckets) {
  NanScope();
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(args.This());
  int64_t bnum; 
  REQUIRE_INT64(args, 0, bnum , "tune_buckets requires first argument to be 64 bit integer")  
  NanReturnValue(NanNew<v8::Boolean>(instance->db->tune_buckets(bnum)));
}
NAN_METHOD(ExportHashDB::tune_map) {
  NanScope();
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(args.This());
  int64_t msiz; 
  REQUIRE_INT64(args, 0, msiz , "tune_map requires first argument to be 64 bit integer")  
  NanReturnValue(NanNew<v8::Boolean>(instance->db->tune_map(msiz)));
}

NAN_METHOD(ExportHashDB::tune_defrag) {
  NanScope();
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(args.This());
  int64_t dfunit; 
  REQUIRE_INT64(args, 0, dfunit , "tune_defrag requires first argument to be 64 bit integer")  
  NanReturnValue(NanNew<v8::Boolean>(instance->db->tune_defrag(dfunit)));
}

