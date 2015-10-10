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
  Nan::SetPrototypeMethod(tpl, "tune_alignment", ExportHashDB::tune_alignment);
  Nan::SetPrototypeMethod(tpl, "tune_fbp", ExportHashDB::tune_fbp);
  Nan::SetPrototypeMethod(tpl, "tune_options", ExportHashDB::tune_options);
  Nan::SetPrototypeMethod(tpl, "tune_buckets", ExportHashDB::tune_buckets);
  Nan::SetPrototypeMethod(tpl, "tune_map", ExportHashDB::tune_map);
  Nan::SetPrototypeMethod(tpl, "tune_defrag", ExportHashDB::tune_defrag);
  v8::Local<v8::Object> Option = Nan::New<v8::Object>();
  Option->Set(Nan::New<v8::String>("TSMALL").ToLocalChecked(), Nan::New<v8::Int32>(kyotocabinet::HashDB::TSMALL));
  Option->Set(Nan::New<v8::String>("TLINEAR").ToLocalChecked(), Nan::New<v8::Int32>(kyotocabinet::HashDB::TLINEAR));
  Option->Set(Nan::New<v8::String>("TCOMPRESS").ToLocalChecked(), Nan::New<v8::Int32>(kyotocabinet::HashDB::TCOMPRESS));
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("Option").ToLocalChecked(), Option);
} 
NAN_METHOD(ExportHashDB::tune_alignment) {
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(info.This());
  int8_t apow; 
  REQUIRE_INT8(info, 0, apow, "tune_alignment requires first argument to be 8 bit integer")  
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->tune_alignment(apow)));
}
NAN_METHOD(ExportHashDB::tune_fbp) {
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(info.This());
  int8_t fpow; 
  REQUIRE_INT8(info, 0, fpow, "tune_fbp requires first argument to be 8 bit integer")  
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->tune_fbp(fpow)));
}
NAN_METHOD(ExportHashDB::tune_options) {
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(info.This());
  int8_t opts; 
  REQUIRE_INT8(info, 0, opts, "tune_options requires first argument to be 8 bit integer")  
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->tune_options(opts)));
}
NAN_METHOD(ExportHashDB::tune_buckets) {
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(info.This());
  int64_t bnum; 
  REQUIRE_INT64(info, 0, bnum , "tune_buckets requires first argument to be 64 bit integer")  
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->tune_buckets(bnum)));
}
NAN_METHOD(ExportHashDB::tune_map) {
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(info.This());
  int64_t msiz; 
  REQUIRE_INT64(info, 0, msiz , "tune_map requires first argument to be 64 bit integer")  
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->tune_map(msiz)));
}

NAN_METHOD(ExportHashDB::tune_defrag) {
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(info.This());
  int64_t dfunit; 
  REQUIRE_INT64(info, 0, dfunit , "tune_defrag requires first argument to be 64 bit integer")  
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->tune_defrag(dfunit)));
}

