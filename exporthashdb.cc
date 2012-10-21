void ExportHashDB::Init(v8::Handle<v8::Object> target, const char * exportName){
  v8::HandleScope scope;
  v8::Local<v8::FunctionTemplate> tpl = PrepareObjectTemlate(exportName);
  setCustomPrototype(tpl);
  ExportTemplate(target, tpl, exportName);  
}
void ExportHashDB::setCustomPrototype(v8::Local<v8::FunctionTemplate> tpl){
  v8::HandleScope scope;
  std::cerr<<"HashDB set prototype"<<std::endl;
  //use backward compatible method to populate prototype 
  //instead of node::SetPrototypeMetod
  NODE_SET_PROTOTYPE_METHOD(tpl, "tune_alignment", ExportHashDB::tune_alignment);
  NODE_SET_PROTOTYPE_METHOD(tpl, "tune_fbp", ExportHashDB::tune_fbp);
  NODE_SET_PROTOTYPE_METHOD(tpl, "tune_options", ExportHashDB::tune_options);
  NODE_SET_PROTOTYPE_METHOD(tpl, "tune_buckets", ExportHashDB::tune_buckets);
  NODE_SET_PROTOTYPE_METHOD(tpl, "tune_map", ExportHashDB::tune_map);
  NODE_SET_PROTOTYPE_METHOD(tpl, "tune_defrag", ExportHashDB::tune_defrag);
  v8::Local<v8::Object> Option = v8::Object::New();
  Option->Set(v8::String::New("TSMALL"), v8::Int32::New(kyotocabinet::HashDB::TSMALL));
  Option->Set(v8::String::New("TLINEAR"), v8::Int32::New(kyotocabinet::HashDB::TLINEAR));
  Option->Set(v8::String::New("TCOMPRESS"), v8::Int32::New(kyotocabinet::HashDB::TCOMPRESS));
  tpl->PrototypeTemplate()->Set(v8::String::New("Option"), Option);
} 
v8::Handle<v8::Value> ExportHashDB::tune_alignment(const v8::Arguments & args){
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(args.This());
  int8_t apow; 
  REQUIRE_INT8(args, 0, apow, "tune_alignment requires first argument to be 8 bit integer")  
  return v8::Boolean::New(instance->db->tune_alignment(apow));
}
v8::Handle<v8::Value> ExportHashDB::tune_fbp(const v8::Arguments & args){
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(args.This());
  int8_t fpow; 
  REQUIRE_INT8(args, 0, fpow, "tune_fbp requires first argument to be 8 bit integer")  
  return v8::Boolean::New(instance->db->tune_fbp(fpow));
}
v8::Handle<v8::Value> ExportHashDB::tune_options(const v8::Arguments & args){
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(args.This());
  int8_t opts; 
  REQUIRE_INT8(args, 0, opts, "tune_options requires first argument to be 8 bit integer")  
  return v8::Boolean::New(instance->db->tune_options(opts));
}
v8::Handle<v8::Value> ExportHashDB::tune_buckets(const v8::Arguments & args){
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(args.This());
  int64_t bnum; 
  REQUIRE_INT64(args, 0, bnum , "tune_buckets requires first argument to be 64 bit integer")  
  return v8::Boolean::New(instance->db->tune_buckets(bnum));
}
v8::Handle<v8::Value> ExportHashDB::tune_map(const v8::Arguments & args){
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(args.This());
  int64_t msiz; 
  REQUIRE_INT64(args, 0, msiz , "tune_map requires first argument to be 64 bit integer")  
  return v8::Boolean::New(instance->db->tune_map(msiz));
}
v8::Handle<v8::Value> ExportHashDB::tune_defrag(const v8::Arguments & args){
  ExportHashDB * instance = node::ObjectWrap::Unwrap<ExportHashDB>(args.This());
  int64_t dfunit; 
  REQUIRE_INT64(args, 0, dfunit , "tune_defrag requires first argument to be 64 bit integer")  
  return v8::Boolean::New(instance->db->tune_defrag(dfunit));
}

