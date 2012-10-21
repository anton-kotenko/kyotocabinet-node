template <class T> v8::Local<v8::FunctionTemplate> ExportToV8<T>::PrepareObjectTemlate(const char * exportName) {
  v8::HandleScope scope;
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(ExportToV8<T>::New);
  tpl->SetClassName(v8::String::NewSymbol(exportName));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  ExportToV8<T>::setDefaultPrototype(tpl);  
  return scope.Close(tpl);
}
template <class T> void  ExportToV8<T>::ExportTemplate(v8::Handle<v8::Object> target, v8::Handle<v8::FunctionTemplate> tpl, const char * exportName) {
  v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
  target->Set(v8::String::NewSymbol(exportName), constructor);
}
template<class T> void ExportToV8<T>::cleanOnShutdown (void * arg) {
  ExportToV8<T>::databasesListNode * next = static_cast<ExportToV8<T>::databasesListNode * >(arg);
  //skip one element (initial next value)
  //as it points to empty node -- begining of list
  while(NULL != (next = next->getNext())) {
    next->deleteContainedItem(); 
  }
}
template<class T> void ExportToV8<T>::registerShutdownCallback() {
  node::AtExit(ExportToV8<T>::cleanOnShutdown, static_cast<void *>(& ExportToV8<T>::listRoot));
}
template <class T> void  ExportToV8<T>::Init(v8::Handle<v8::Object> target, const char * exportName) {
  v8::HandleScope scope;
  v8::Local<v8::FunctionTemplate> tpl = PrepareObjectTemlate(exportName);
  ExportTemplate(target, tpl, exportName); 
  registerShutdownCallback();
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::New(const v8::Arguments & args) {
  ExportToV8<T> * instance = new ExportToV8<T>();
  instance->Wrap(args.This());
  return args.This();
}
template <class T> ExportToV8<T>::ExportToV8(){
  this->db = new T();
  this->me = ExportToV8<T>::listRoot.attachNew(this);
}
template <class T> ExportToV8<T>::~ExportToV8(){
  if(this->db) {
    //this call's destructor of this->db 
    //destructor implicitly closes database
    delete this->db;
    this->db = NULL;
  }
  if(this->me) {
    delete this->me;
    this->me = NULL;
  }
}
template <class T> typename ExportToV8<T>::databasesListNode ExportToV8<T>::listRoot;
template <class T> void ExportToV8<T>::setDefaultPrototype(v8::Local<v8::FunctionTemplate> tpl){
  v8::HandleScope scope;
  //use backward compatible method to populate prototype 
  //instead of node::SetPrototypeMetod
  NODE_SET_PROTOTYPE_METHOD(tpl, "open", ExportToV8<T>::open);
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", ExportToV8<T>::close);
  NODE_SET_PROTOTYPE_METHOD(tpl, "size", ExportToV8<T>::size);
  NODE_SET_PROTOTYPE_METHOD(tpl, "count", ExportToV8<T>::count);
  NODE_SET_PROTOTYPE_METHOD(tpl, "path", ExportToV8<T>::path);
  NODE_SET_PROTOTYPE_METHOD(tpl, "clear", ExportToV8<T>::clear);
  NODE_SET_PROTOTYPE_METHOD(tpl, "get", ExportToV8<T>::get);
  NODE_SET_PROTOTYPE_METHOD(tpl, "set", ExportToV8<T>::set);
  NODE_SET_PROTOTYPE_METHOD(tpl, "add", ExportToV8<T>::add);
  NODE_SET_PROTOTYPE_METHOD(tpl, "replace", ExportToV8<T>::replace);
  NODE_SET_PROTOTYPE_METHOD(tpl, "remove", ExportToV8<T>::remove);
  NODE_SET_PROTOTYPE_METHOD(tpl, "check", ExportToV8<T>::check);
  NODE_SET_PROTOTYPE_METHOD(tpl, "seize", ExportToV8<T>::seize);
  NODE_SET_PROTOTYPE_METHOD(tpl, "cas", ExportToV8<T>::cas);
  NODE_SET_PROTOTYPE_METHOD(tpl, "increment", ExportToV8<T>::increment);
  NODE_SET_PROTOTYPE_METHOD(tpl, "append", ExportToV8<T>::append);
  NODE_SET_PROTOTYPE_METHOD(tpl, "set_bulk", ExportToV8<T>::set_bulk);
  NODE_SET_PROTOTYPE_METHOD(tpl, "remove_bulk", ExportToV8<T>::remove_bulk);
  NODE_SET_PROTOTYPE_METHOD(tpl, "get_bulk", ExportToV8<T>::get_bulk);
  NODE_SET_PROTOTYPE_METHOD(tpl, "status", ExportToV8<T>::status);
  NODE_SET_PROTOTYPE_METHOD(tpl, "begin_transaction", ExportToV8<T>::begin_transaction);
  NODE_SET_PROTOTYPE_METHOD(tpl, "begin_transaction_try", ExportToV8<T>::begin_transaction_try);
  NODE_SET_PROTOTYPE_METHOD(tpl, "end_transaction", ExportToV8<T>::end_transaction);
  NODE_SET_PROTOTYPE_METHOD(tpl, "error", ExportToV8<T>::error);
  NODE_SET_PROTOTYPE_METHOD(tpl, "copy", ExportToV8<T>::copy);
  NODE_SET_PROTOTYPE_METHOD(tpl, "accept", ExportToV8<T>::accept);
  NODE_SET_PROTOTYPE_METHOD(tpl, "accept_bulk", ExportToV8<T>::accept_bulk);
  v8::Local<v8::Object> OpenMode = v8::Object::New();
  OpenMode->Set(v8::String::New("OREADER"), v8::Int32::New(T::OREADER));
  OpenMode->Set(v8::String::New("OWRITER"), v8::Int32::New(T::OWRITER));
  OpenMode->Set(v8::String::New("OCREATE"), v8::Int32::New(T::OCREATE));
  OpenMode->Set(v8::String::New("OTRUNCATE"), v8::Int32::New(T::OTRUNCATE));
  OpenMode->Set(v8::String::New("OAUTOTRAN"), v8::Int32::New(T::OAUTOTRAN));
  OpenMode->Set(v8::String::New("OAUTOSYNC"), v8::Int32::New(T::OAUTOSYNC));
  OpenMode->Set(v8::String::New("ONOLOCK"), v8::Int32::New(T::ONOLOCK));
  OpenMode->Set(v8::String::New("OTRYLOCK"), v8::Int32::New(T::OTRYLOCK));
  OpenMode->Set(v8::String::New("ONOREPAIR"), v8::Int32::New(T::ONOREPAIR));
  tpl->PrototypeTemplate()->Set(v8::String::New("OpenMode"), OpenMode);
  v8::Local<v8::Object> ErrorCode = v8::Object::New();
  ErrorCode->Set(v8::String::New("SUCCESS"), v8::Int32::New(T::Error::SUCCESS));
  ErrorCode->Set(v8::String::New("NOIMPL"), v8::Int32::New(T::Error::NOIMPL));
  ErrorCode->Set(v8::String::New("INVALID"), v8::Int32::New(T::Error::INVALID));
  ErrorCode->Set(v8::String::New("NOREPOS"), v8::Int32::New(T::Error::NOREPOS));
  ErrorCode->Set(v8::String::New("NOPERM"), v8::Int32::New(T::Error::NOPERM));
  ErrorCode->Set(v8::String::New("BROKEN"), v8::Int32::New(T::Error::BROKEN));
  ErrorCode->Set(v8::String::New("NOREC"), v8::Int32::New(T::Error::NOREC));
  ErrorCode->Set(v8::String::New("DUPREC"), v8::Int32::New(T::Error::DUPREC));
  ErrorCode->Set(v8::String::New("LOGIC"), v8::Int32::New(T::Error::LOGIC));
  ErrorCode->Set(v8::String::New("SYSTEM"), v8::Int32::New(T::Error::SYSTEM));
  ErrorCode->Set(v8::String::New("MISC"), v8::Int32::New(T::Error::MISC));
  tpl->PrototypeTemplate()->Set(v8::String::New("ErrorCode"), ErrorCode);
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::open(const v8::Arguments & args) {
  std::string path;
  int mode;
  REQUIRE_STRING(args, 0, path, "open requires first argument to be string")
  REQUIRE_INT32(args, 1, mode, "open requires second argument to be integer")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Boolean::New(instance->db->open(path, mode));
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::close(const v8::Arguments & args) {
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Boolean::New(instance->db->close());
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::size(const v8::Arguments & args) {
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Integer::New(instance->db->size());
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::count(const v8::Arguments & args) {
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Integer::New(instance->db->count());
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::path(const v8::Arguments & args) {
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::String::New(instance->db->path().c_str());
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::clear(const v8::Arguments & args) {
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Boolean::New(instance->db->clear());
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::get(const v8::Arguments & args) {
  std::string key;
  std::string value; 
  REQUIRE_STRING(args, 0, key, "get requires first argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  if (!instance->db->get(key, &value)) {
    return v8::Undefined();
  } 
  return v8::String::New(value.c_str(), value.size());
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::set(const v8::Arguments & args) {
  v8::HandleScope scope;
  v8::Local<v8::Object> key, value;
  REQUIRE_BUFFER(args, 0, key, "set requires first argument to be buffer")
  REQUIRE_BUFFER(args, 1, value, "set requires second argument to be buffer")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return scope.Close(v8::Boolean::New(instance->db->set(BUFFER_DATA_LENGTH(key), BUFFER_DATA_LENGTH(value)))); 
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::add(const v8::Arguments & args) {
  v8::HandleScope scope;
  v8::Local<v8::Object> key, value;
  REQUIRE_BUFFER(args, 0, key, "add requires first argument to be buffer")
  REQUIRE_BUFFER(args, 1, value, "add requires second argument to be buffer")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return scope.Close(v8::Boolean::New(instance->db->add(BUFFER_DATA_LENGTH(key), BUFFER_DATA_LENGTH(value)))); 
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::replace(const v8::Arguments & args) {
  v8::HandleScope scope;
  v8::Local<v8::Object> key, value;
  REQUIRE_BUFFER(args, 0, key, "replace requires first argument to be buffer")
  REQUIRE_BUFFER(args, 1, value, "replace requires second argument to be buffer")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return scope.Close(v8::Boolean::New(instance->db->replace(BUFFER_DATA_LENGTH(key), BUFFER_DATA_LENGTH(value)))); 
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::remove(const v8::Arguments & args) {
  v8::HandleScope scope;
  v8::Local<v8::Object> key;
  REQUIRE_BUFFER(args, 0, key, "remove requires first argument to be buffer")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return scope.Close(v8::Boolean::New(instance->db->remove(BUFFER_DATA_LENGTH(key))));
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::check(const v8::Arguments & args) {
  v8::HandleScope scope;
  v8::Local<v8::Object> key;
  REQUIRE_BUFFER(args, 0, key, "check requires first argument to be buffer")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return scope.Close(v8::Int32::New(instance->db->check(BUFFER_DATA_LENGTH(key))));
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::seize(const v8::Arguments & args) {
  v8::HandleScope scope;
  v8::Local<v8::Object> key, value;
  size_t value_length;
  char * value_data;
  REQUIRE_BUFFER(args, 0, key, "seize requires first argument to be buffer")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  value_data = instance->db->seize(BUFFER_DATA_LENGTH(key), &value_length);
  if (!value_data){
    return v8::Undefined(); 
  }
  value = *(node::Buffer::New(value_data, value_length)->handle_);
  delete value_data;
  return scope.Close(value);
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::cas(const v8::Arguments & args) {
  v8::HandleScope scope;
  v8::Local<v8::Object> key, ovalue, nvalue;
  REQUIRE_BUFFER(args, 0, key, "cas requires first argument to be buffer")
  REQUIRE_BUFFER(args, 1, ovalue, "cas requires second argument to be buffer")
  REQUIRE_BUFFER(args, 2, nvalue, "cas requires third argument to be buffer")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return scope.Close(v8::Boolean::New(instance->db->cas(BUFFER_DATA_LENGTH(key), BUFFER_DATA_LENGTH(ovalue), BUFFER_DATA_LENGTH(nvalue)))); 
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::increment(const v8::Arguments & args) {
  v8::HandleScope scope;
  v8::Local<v8::Object> key;
  int64_t intIncrement;
  int64_t intOriginal = 0;
  double doubleIncrement;
  double doubleOriginal = 0;
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  REQUIRE_BUFFER(args, 0, key, "increment requires first argument to be buffer")
  if (args.Length() < 2) {
    return v8::ThrowException(v8::Exception::Error(v8::String::New("increment function requires at least 2 arguments")));
  }
  if (args[1]->IsInt32()) {
    REQUIRE_INT64(args, 1, intIncrement, "increments second argument is not integer")
    if (args.Length()>=3) {
      REQUIRE_INT64(args, 2, intOriginal, "increments third argument has to be integer if first is integer")
    }
    return scope.Close(v8::Boolean::New(instance->db->increment(BUFFER_DATA_LENGTH(key), intIncrement, intOriginal)));
  } else if(args[1]->IsNumber()) {
    REQUIRE_DOUBLE(args, 1, doubleIncrement, "increments second argument is not double")
    if (args.Length() >= 3) {
      REQUIRE_INT64(args, 2, doubleOriginal, "increments third argument has to be double if first is double")
    }
    return scope.Close(v8::Boolean::New(instance->db->increment_double(BUFFER_DATA_LENGTH(key), doubleIncrement, doubleOriginal)));
  } 
  return v8::ThrowException(v8::Exception::Error(v8::String::New("increment function requires second argument to be integer or double")));
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::append(const v8::Arguments & args) {
  v8::HandleScope scope;
  v8::Local<v8::Object> key, value;
  REQUIRE_BUFFER(args, 0, key, "append requires first argument to be buffer")
  REQUIRE_BUFFER(args, 1, value, "append requires second argument to be buffer")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return scope.Close(v8::Boolean::New(instance->db->append(BUFFER_DATA_LENGTH(key), BUFFER_DATA_LENGTH(value)))); 
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::set_bulk(const v8::Arguments & args) {
  //this function uses strings instead of buffers because
  //1. set_bulk uses std::map<std::string, std::string>
  //2. buffer can't be object key (even that has to be true in future versions of JS)  
  v8::HandleScope scope;
  std::map<std::string, std::string> data;
  bool atomic = true;
  v8::Local<v8::Object> bulk; 
  v8::Local<v8::Array> keys;  
  REQUIRE_OBJECT(args, 0, bulk, "set_bulk requires first argument to be object")
  if (args.Length()>=2) {
    REQUIRE_BOOLEAN(args, 1, atomic, "set_bulk requires second argument to be boolean")
  }
  keys = bulk->GetOwnPropertyNames();
  for (unsigned int i = 0; i < keys->Length(); i++) {
    data[*v8::String::Utf8Value(keys->Get(i))] = *v8::String::Utf8Value(bulk->Get(keys->Get(i)));
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return scope.Close(v8::Integer::New(instance->db->set_bulk(data, atomic)));
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::remove_bulk(const v8::Arguments & args) {
  //this function uses strings instead of buffers because
  //remove_bulk uses std::vector<std::string>
  std::vector<std::string> data;
  bool atomic = true;
  v8::HandleScope scope;
  v8::Local<v8::Array> keys;  
  REQUIRE_ARRAY(args, 0, keys, "remove_bulk requires first argument to be array")
  if (args.Length() >= 2) {
    REQUIRE_BOOLEAN(args, 1, atomic, "remove_bulk requires second argument to be boolean")
  }
  data.resize(keys->Length());
  for ( unsigned int i=0; i<keys->Length(); i++) {
    data[i] = *v8::String::Utf8Value(keys->Get(i));
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return scope.Close(v8::Integer::New(instance->db->remove_bulk(data, atomic))); 
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::get_bulk(const v8::Arguments & args) {
  //this function uses strings instead of buffers because
  //get_bulk uses std::vector<std::string>
  std::vector<std::string> keysVector;
  std::map<std::string, std::string> valuesMap;
  bool atomic = true;
  v8::HandleScope scope;
  v8::Local<v8::Array> keys;  
  v8::Local<v8::Object> values = v8::Object::New();
  REQUIRE_ARRAY(args, 0, keys, "get_bulk requires first argument to be array")
  if (args.Length()>=2) {
    REQUIRE_BOOLEAN(args, 1, atomic, "get_bulk requires second argument to be boolean")
  }
  keysVector.resize(keys->Length());
  for ( unsigned int i=0; i<keys->Length(); i++) {
    keysVector[i] = *v8::String::Utf8Value(keys->Get(i));
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  if(-1 == instance->db->get_bulk(keysVector, &valuesMap, atomic)) {
    return v8::Undefined();
  }
  for (std::map<std::string, std::string>::const_iterator i = valuesMap.begin(); i != valuesMap.end(); i++) {
    values->Set(v8::String::New(i->first.c_str()), v8::String::New(i->second.c_str()));
  }
  return scope.Close(values);
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::status(const v8::Arguments & args) {
  std::map<std::string, std::string> valueMap;
  v8::HandleScope scope;
  v8::Local<v8::Object> value = v8::Object::New();
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  if(!instance->db->status(&valueMap)) {
    return scope.Close(v8::Undefined());
  }
  for(std::map<std::string, std::string>::const_iterator i = valueMap.begin(); i!=valueMap.end(); i++ ){
    value->Set(v8::String::New(i->first.c_str()), v8::String::New(i->second.c_str()));
  }
  return scope.Close(value);
}
/*#include <iostream>
using namespace std;
template <class T> class CopyWork: public standartProgressChecker<T>{
  private:
  static void work_cb(uv_work_t * req) {
    CopyWork<T> * this_obj = static_cast<CopyWork<T>* >(req->data);
    std::string path;
    REQUIRE_STRING(args, 0, path, "copy requires first argument to be string")
    this_obj->_db->copy(_path, &this_obj); 
  }
};*/
template <class T> v8::Handle<v8::Value> ExportToV8<T>::copy(const v8::Arguments & args) {
 // ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  v8::HandleScope scope;
 // (new CopyWork<T>(instance->db, path, callback))->run();
  return scope.Close(v8::True());
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::begin_transaction(const v8::Arguments & args) {
  bool hard = false;
  if (args.Length() > 0) {
    REQUIRE_BOOLEAN(args, 0, hard, "begin_transaction requires first argument to be boolean");
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Boolean::New(instance->db->begin_transaction(hard));
}

template <class T> v8::Handle<v8::Value> ExportToV8<T>::begin_transaction_try(const v8::Arguments & args) {
  bool hard = false;
  if (args.Length() > 0) {
    REQUIRE_BOOLEAN(args, 0, hard, "begin_transaction_try requires first argument to be boolean");
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Boolean::New(instance->db->begin_transaction_try(hard));
}

template <class T> v8::Handle<v8::Value> ExportToV8<T>::end_transaction(const v8::Arguments & args) {
  bool commit = true;
  if (args.Length() > 0) {
    REQUIRE_BOOLEAN(args, 0, commit, "end_transaction requires first argument to be boolean");
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Boolean::New(instance->db->end_transaction(commit));
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::error(const v8::Arguments & args) {
  v8::HandleScope scope;
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  typename T::Error kcError = instance->db->error();
  v8::Local<v8::Object> v8Error = v8::Object::Cast(*v8::Exception::Error(v8::String::New(kcError.message())));
  v8Error->Set(v8::String::New("name"), v8::String::New(kcError.name()));
  v8Error->Set(v8::String::New("code"), v8::Int32::New(kcError.code()));
  return scope.Close(v8Error);
}
template <class T>  v8::Handle<v8::Value> ExportToV8<T>::accept(const v8::Arguments & args) {
  v8::HandleScope scope;
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  v8::Local<v8::Object> v8Visitor;
  std::string key;
  bool writable = true;
  REQUIRE_STRING(args, 0, key, "accept method requires first argument to be string");
  REQUIRE_OBJECT(args, 1, v8Visitor, "accept method requires second argument to be Object");
  if (args.Length() > 2) {
    REQUIRE_BOOLEAN(args, 2, writable, "accept method requires third argument to be boolean, or not to be at all"); 
  }  
  ExportVisitor visitor(v8Visitor);
  return scope.Close(v8::Boolean::New(instance->db->accept(key.c_str(), key.size(), &visitor, writable))); 
}
template <class T>  v8::Handle<v8::Value> ExportToV8<T>::accept_bulk(const v8::Arguments & args) {
  v8::HandleScope scope;
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  v8::Local<v8::Object> v8Visitor;
  v8::Local<v8::Array> v8Keys;
  bool writable = true;
  REQUIRE_ARRAY(args, 0, v8Keys, "accept_bulk method requires first argument to be an array of strings");
  REQUIRE_OBJECT(args, 1, v8Visitor, "accept_bulk method requires second argument to be Object");
  if (args.Length() > 2) {
    REQUIRE_BOOLEAN(args, 2, writable, "accept method requires third argument to be boolean, or not to be at all"); 
  }  
  std::vector<std::string> keys(v8Keys->Length());
  for (unsigned int i = 0; i < v8Keys->Length(); i++) {
    keys[i] = *v8::String::Utf8Value(v8Keys->Get(i)->ToString());
  }
  ExportVisitor visitor(v8Visitor);
  return scope.Close(v8::Boolean::New(instance->db->accept_bulk(keys, &visitor, writable))); 
}

