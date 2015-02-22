#include <string>
#include <map>
#include <vector>

#include <v8.h>

#include "tools.h"
#include "progress_checker.h"

template <class T> v8::Local<v8::FunctionTemplate> ExportToV8<T>::PrepareObjectTemlate(const char * exportName) {
  v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(ExportToV8<T>::New);
  tpl->SetClassName(NanNew<v8::String>(exportName));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  ExportToV8<T>::setDefaultPrototype(tpl);  
  return tpl;
}

template <class T> void  ExportToV8<T>::ExportTemplate(v8::Handle<v8::Object> target, v8::Handle<v8::FunctionTemplate> tpl, const char * exportName) {
  target->Set(NanNew<v8::String>(exportName), tpl->GetFunction());
}

template <class T> void  ExportToV8<T>::Init(v8::Handle<v8::Object> target, const char * exportName) {
  v8::Local<v8::FunctionTemplate> tpl = PrepareObjectTemlate(exportName);
  ExportTemplate(target, tpl, exportName); 
};

template <class T> NAN_METHOD(ExportToV8<T>::New) {
  NanScope();
  ExportToV8<T> * instance = new ExportToV8<T>();
  instance->Wrap(args.This());
  NanReturnThis();
}

template <class T> ExportToV8<T>::ExportToV8(){
  this->db = new T();
}

template <class T> ExportToV8<T>::~ExportToV8(){
  if (this->db) {
    delete this->db;
    this->db = NULL;
  }
};

template <class T> void ExportToV8<T>::setDefaultPrototype(v8::Local<v8::FunctionTemplate> tpl){
  NODE_SET_PROTOTYPE_METHOD(tpl, "open", ExportToV8<T>::open);
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", ExportToV8<T>::close);
  NODE_SET_PROTOTYPE_METHOD(tpl, "size", ExportToV8<T>::size);
  NODE_SET_PROTOTYPE_METHOD(tpl, "count", ExportToV8<T>::count);
  NODE_SET_PROTOTYPE_METHOD(tpl, "path", ExportToV8<T>::path);
  NODE_SET_PROTOTYPE_METHOD(tpl, "status", ExportToV8<T>::status);
  NODE_SET_PROTOTYPE_METHOD(tpl, "error", ExportToV8<T>::error);

  NODE_SET_PROTOTYPE_METHOD(tpl, "clear", ExportToV8<T>::clear);
  NODE_SET_PROTOTYPE_METHOD(tpl, "copy", ExportToV8<T>::copy);

  NODE_SET_PROTOTYPE_METHOD(tpl, "get", ExportToV8<T>::get);
  NODE_SET_PROTOTYPE_METHOD(tpl, "set", ExportToV8<T>::set);
  NODE_SET_PROTOTYPE_METHOD(tpl, "add", ExportToV8<T>::add);
  NODE_SET_PROTOTYPE_METHOD(tpl, "replace", ExportToV8<T>::replace);
  NODE_SET_PROTOTYPE_METHOD(tpl, "append", ExportToV8<T>::append);
  NODE_SET_PROTOTYPE_METHOD(tpl, "remove", ExportToV8<T>::remove);
  NODE_SET_PROTOTYPE_METHOD(tpl, "check", ExportToV8<T>::check);
  NODE_SET_PROTOTYPE_METHOD(tpl, "increment", ExportToV8<T>::increment);
  NODE_SET_PROTOTYPE_METHOD(tpl, "seize", ExportToV8<T>::seize);
  NODE_SET_PROTOTYPE_METHOD(tpl, "cas", ExportToV8<T>::cas);

  NODE_SET_PROTOTYPE_METHOD(tpl, "set_bulk", ExportToV8<T>::set_bulk);
  NODE_SET_PROTOTYPE_METHOD(tpl, "remove_bulk", ExportToV8<T>::remove_bulk);
  NODE_SET_PROTOTYPE_METHOD(tpl, "get_bulk", ExportToV8<T>::get_bulk);
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "begin_transaction", ExportToV8<T>::begin_transaction);
  NODE_SET_PROTOTYPE_METHOD(tpl, "begin_transaction_try", ExportToV8<T>::begin_transaction_try);
  NODE_SET_PROTOTYPE_METHOD(tpl, "end_transaction", ExportToV8<T>::end_transaction);

  NODE_SET_PROTOTYPE_METHOD(tpl, "cursor", ExportToV8<T>::cursor);

  v8::Local<v8::Object> OpenMode = NanNew<v8::Object>();
  OpenMode->Set(NanNew<v8::String>("OREADER"), NanNew<v8::Int32>(T::OREADER));
  OpenMode->Set(NanNew<v8::String>("OWRITER"), NanNew<v8::Int32>(T::OWRITER));
  OpenMode->Set(NanNew<v8::String>("OCREATE"), NanNew<v8::Int32>(T::OCREATE));
  OpenMode->Set(NanNew<v8::String>("OTRUNCATE"), NanNew<v8::Int32>(T::OTRUNCATE));
  OpenMode->Set(NanNew<v8::String>("OAUTOTRAN"), NanNew<v8::Int32>(T::OAUTOTRAN));
  OpenMode->Set(NanNew<v8::String>("OAUTOSYNC"), NanNew<v8::Int32>(T::OAUTOSYNC));
  OpenMode->Set(NanNew<v8::String>("ONOLOCK"), NanNew<v8::Int32>(T::ONOLOCK));
  OpenMode->Set(NanNew<v8::String>("OTRYLOCK"), NanNew<v8::Int32>(T::OTRYLOCK));
  OpenMode->Set(NanNew<v8::String>("ONOREPAIR"), NanNew<v8::Int32>(T::ONOREPAIR));
  tpl->PrototypeTemplate()->Set(NanNew<v8::String>("OpenMode"), OpenMode);
  v8::Local<v8::Object> ErrorCode = NanNew<v8::Object>();
  ErrorCode->Set(NanNew<v8::String>("SUCCESS"), NanNew<v8::Int32>(T::Error::SUCCESS));
  ErrorCode->Set(NanNew<v8::String>("NOIMPL"), NanNew<v8::Int32>(T::Error::NOIMPL));
  ErrorCode->Set(NanNew<v8::String>("INVALID"), NanNew<v8::Int32>(T::Error::INVALID));
  ErrorCode->Set(NanNew<v8::String>("NOREPOS"), NanNew<v8::Int32>(T::Error::NOREPOS));
  ErrorCode->Set(NanNew<v8::String>("NOPERM"), NanNew<v8::Int32>(T::Error::NOPERM));
  ErrorCode->Set(NanNew<v8::String>("BROKEN"), NanNew<v8::Int32>(T::Error::BROKEN));
  ErrorCode->Set(NanNew<v8::String>("NOREC"), NanNew<v8::Int32>(T::Error::NOREC));
  ErrorCode->Set(NanNew<v8::String>("DUPREC"), NanNew<v8::Int32>(T::Error::DUPREC));
  ErrorCode->Set(NanNew<v8::String>("LOGIC"), NanNew<v8::Int32>(T::Error::LOGIC));
  ErrorCode->Set(NanNew<v8::String>("SYSTEM"), NanNew<v8::Int32>(T::Error::SYSTEM));
  ErrorCode->Set(NanNew<v8::String>("MISC"), NanNew<v8::Int32>(T::Error::MISC));
  tpl->PrototypeTemplate()->Set(NanNew<v8::String>("ErrorCode"), ErrorCode);
}

template <class T> NAN_METHOD(ExportToV8<T>::open) {
  NanScope();
  std::string path;
  int mode;
  REQUIRE_STRING(args, 0, path, "open requires first argument to be string")
  REQUIRE_INT32(args, 1, mode, "open requires second argument to be integer")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  NanReturnValue(NanNew<v8::Boolean>(instance->db->open(path, mode)));
}

template <class T> NAN_METHOD(ExportToV8<T>::close) {
  NanScope();
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  NanReturnValue(NanNew<v8::Boolean>(instance->db->close()));
}

template <class T> NAN_METHOD(ExportToV8<T>::size) {
  NanScope();
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  //FIXME that's BAD: what if size is greater than uint_32?
  NanReturnValue(NanNew<v8::Integer>(static_cast<uint32_t>(instance->db->size())));
}

template <class T> NAN_METHOD(ExportToV8<T>::count) {
  NanScope();
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  //FIXME that's BAD: what if count is greater than uint_32?
  NanReturnValue(NanNew<v8::Integer>(static_cast<uint32_t>(instance->db->count())));
}

template <class T> NAN_METHOD(ExportToV8<T>::path) {
  NanScope();
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  NanReturnValue(NanNew<v8::String>(instance->db->path().c_str()));
}

template <class T> NAN_METHOD(ExportToV8<T>::status) {
  NanScope();
  std::map<std::string, std::string> valueMap;
  v8::Local<v8::Object> value = NanNew<v8::Object>();
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  if(!instance->db->status(&valueMap)) {
    NanReturnUndefined();
  }
  for(std::map<std::string, std::string>::const_iterator i = valueMap.begin(); i!=valueMap.end(); i++ ){
    value->Set(NanNew<v8::String>(i->first.c_str()), NanNew<v8::String>(i->second.c_str()));
  }
  NanReturnValue(value);
}

template <class T> NAN_METHOD(ExportToV8<T>::error) {
  NanScope();
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  typename T::Error kcError = instance->db->error();
  v8::Local<v8::Object> v8Error = v8::Local<v8::Object>::Cast(v8::Exception::Error(NanNew<v8::String>(kcError.message())));
  v8Error->Set(NanNew<v8::String>("name"), NanNew<v8::String>(kcError.name()));
  v8Error->Set(NanNew<v8::String>("code"), NanNew<v8::Int32>(kcError.code()));
  NanReturnValue(v8Error);
}

template <class T> NAN_METHOD(ExportToV8<T>::clear) {
  NanScope();
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  NanReturnValue(NanNew<v8::Boolean>(instance->db->clear()));
}

template <class T> NAN_METHOD(ExportToV8<T>::copy) {
  NanScope();
  std::string path;
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  //v8::Local<v8::Object> callback = args[1]->ToObject();
  StandartProgressChecker * checker = new StandartProgressChecker(/*callback*/);
  REQUIRE_STRING(args, 0, path, "copy requires first argument to be string")
  bool result = instance->db->copy(path, checker);
  delete checker;
  NanReturnValue(NanNew<v8::Boolean>(result));
}


template <class T> NAN_METHOD(ExportToV8<T>::get) {
  NanScope();
  std::string key;
  std::string value; 
  REQUIRE_STRING(args, 0, key, "get requires first argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  if (!instance->db->get(key, &value)) {
    NanReturnUndefined();
  } 
  NanReturnValue(NanNew<v8::String>(value.c_str()));
}

template <class T> NAN_METHOD(ExportToV8<T>::set) {
  NanScope();
  std::string key;
  std::string value; 
  REQUIRE_STRING(args, 0, key, "set requires first argument to be string")
  REQUIRE_STRING(args, 1, value, "set requires second argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  NanReturnValue(NanNew<v8::Boolean>(instance->db->set(key, value))); 
}

template <class T> NAN_METHOD(ExportToV8<T>::add) {
  NanScope();
  std::string key;
  std::string value; 
  REQUIRE_STRING(args, 0, key, "add requires first argument to be string")
  REQUIRE_STRING(args, 1, value, "add requires second argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  NanReturnValue(NanNew<v8::Boolean>(instance->db->add(key, value))); 
}

template <class T> NAN_METHOD(ExportToV8<T>::replace) {
  NanScope();
  std::string key;
  std::string value; 
  REQUIRE_STRING(args, 0, key, "replace requires first argument to be string")
  REQUIRE_STRING(args, 1, value, "replace requires second argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  NanReturnValue(NanNew<v8::Boolean>(instance->db->replace(key, value))); 
}

template <class T> NAN_METHOD(ExportToV8<T>::append) {
  NanScope();
  std::string key;
  std::string value; 
  REQUIRE_STRING(args, 0, key, "append requires first argument to be string")
  REQUIRE_STRING(args, 1, value, "append requires second argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  NanReturnValue(NanNew<v8::Boolean>(instance->db->append(key, value))); 
}

template <class T> NAN_METHOD(ExportToV8<T>::remove) {
  NanScope();
  std::string key;
  REQUIRE_STRING(args, 0, key, "remove requires first argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  NanReturnValue(NanNew<v8::Boolean>(instance->db->remove(key)));
}

template <class T> NAN_METHOD(ExportToV8<T>::check) {
  NanScope();
  std::string key;
  REQUIRE_STRING(args, 0, key, "check requires first argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  NanReturnValue(NanNew<v8::Int32>(instance->db->check(key)));
}

template <class T> NAN_METHOD(ExportToV8<T>::increment) {
  NanScope();
  std::string key;
  int64_t intIncrement;
  int64_t intOriginal = 0;
  double doubleIncrement;
  double doubleOriginal = 0;
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  REQUIRE_STRING(args, 0, key, "increment requires first argument to be string")
  if (args.Length()<2) {
    NanReturnValue(V8_THROW(v8::Exception::Error(
      NanNew<v8::String>("increment function requires at least 2 arguments")
    )));
  }
  if (args[1]->IsInt32()) {
    REQUIRE_INT64(args, 1, intIncrement, "increments second argument is not integer")
    if (args.Length()>=3) {
      REQUIRE_INT64(args, 2, intOriginal, "increments third argument has to be integer if first is integer")
    }
    NanReturnValue(NanNew<v8::Boolean>(instance->db->increment(key, intIncrement, intOriginal)));
  } else if(args[1]->IsNumber()) {
    REQUIRE_DOUBLE(args, 1, doubleIncrement, "increments second argument is not double")
    if (args.Length()>=3) {
      REQUIRE_INT64(args, 2, doubleOriginal, "increments third argument has to be double if first is double")
    }
    NanReturnValue(NanNew<v8::Boolean>(instance->db->increment_double(key, doubleIncrement, doubleOriginal)));
  } 

  NanReturnValue(V8_THROW(v8::Exception::Error(
    NanNew<v8::String>("increment function requires second argument to be integer or double")
  )));
}

template <class T> NAN_METHOD(ExportToV8<T>::seize) {
  NanScope();
  std::string key;
  std::string value; 
  REQUIRE_STRING(args, 0, key, "seize requires first argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  if (!instance->db->seize(key, &value)) {
    NanReturnUndefined();
  }
  NanReturnValue(NanNew<v8::String>(value.c_str())); 
}

template <class T> NAN_METHOD(ExportToV8<T>::cas) {
  NanScope();
  std::string key;
  std::string ovalue; 
  std::string nvalue; 
  REQUIRE_STRING(args, 0, key, "cas requires first argument to be string")
  REQUIRE_STRING(args, 1, ovalue, "cas requires second argument to be string")
  REQUIRE_STRING(args, 2, nvalue, "cas requires third argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  NanReturnValue(NanNew<v8::Boolean>(instance->db->cas(key, ovalue, nvalue))); 
}

template <class T> NAN_METHOD(ExportToV8<T>::set_bulk) {
  NanScope();
  std::map<std::string, std::string> data;
  bool atomic = true;
  v8::Local<v8::Object> bulk; 
  v8::Local<v8::Array> keys;  
  REQUIRE_OBJECT(args, 0, bulk, "set_bulk requires first argument to be object")
  if (args.Length()>=2) {
    REQUIRE_BOOLEAN(args, 1, atomic, "set_bulk requires second argument to be boolean")
  }
  keys = bulk->GetOwnPropertyNames();
  for ( unsigned int i=0; i<keys->Length(); i++) {
    data[*v8::String::Utf8Value(keys->Get(i))] = *v8::String::Utf8Value(bulk->Get(keys->Get(i)));
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  //FIXME that's BAD: what if size is greater than uint_32?
  NanReturnValue(NanNew<v8::Integer>(static_cast<uint32_t>(instance->db->set_bulk(data, atomic))));
}

template <class T> NAN_METHOD(ExportToV8<T>::remove_bulk) {
  NanScope();
  std::vector<std::string> data;
  bool atomic = true;
  v8::Local<v8::Array> keys;  
  REQUIRE_ARRAY(args, 0, keys, "remove_bulk requires first argument to be array")
  if (args.Length()>=2) {
    REQUIRE_BOOLEAN(args, 1, atomic, "remove_bulk requires second argument to be boolean")
  }
  data.resize(keys->Length());
  for ( unsigned int i=0; i<keys->Length(); i++) {
    data[i] = *v8::String::Utf8Value(keys->Get(i));
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  //FIXME that's BAD: what if size is greater than uint_32?
  NanReturnValue(NanNew<v8::Integer>(static_cast<uint32_t>(instance->db->remove_bulk(data, atomic)))); 
}

template <class T> NAN_METHOD(ExportToV8<T>::get_bulk) {
  NanScope();
  std::vector<std::string> keysVector;
  std::map<std::string, std::string> valuesMap;
  bool atomic = true;
  v8::Local<v8::Array> keys;  
  v8::Local<v8::Object> values = NanNew<v8::Object>();

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
    NanReturnUndefined();
  }
  for (std::map<std::string, std::string>::const_iterator i = valuesMap.begin(); i != valuesMap.end(); i++) {
    values->Set(NanNew<v8::String>(i->first.c_str()), NanNew<v8::String>(i->second.c_str()));
  }
  NanReturnValue(values);
}

template <class T> NAN_METHOD(ExportToV8<T>::begin_transaction) {
  NanScope();
  bool hard = false;
  if (args.Length() > 0) {
    REQUIRE_BOOLEAN(args, 0, hard, "begin_transaction requires first argument to be boolean");
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  NanReturnValue(NanNew<v8::Boolean>(instance->db->begin_transaction(hard)));
}

template <class T> NAN_METHOD(ExportToV8<T>::begin_transaction_try) {
  NanScope();
  bool hard = false;
  if (args.Length() > 0) {
    REQUIRE_BOOLEAN(args, 0, hard, "begin_transaction_try requires first argument to be boolean");
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  NanReturnValue(NanNew<v8::Boolean>(instance->db->begin_transaction_try(hard)));
}

template <class T> NAN_METHOD(ExportToV8<T>::end_transaction) {
  NanScope();
  bool commit = true;
  if (args.Length() > 0) {
    REQUIRE_BOOLEAN(args, 0, commit, "end_transaction requires first argument to be boolean");
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  NanReturnValue(NanNew<v8::Boolean>(instance->db->end_transaction(commit)));
}

template <class T> NAN_METHOD(ExportToV8<T>::cursor) {
  NanScope();
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  kyotocabinet::DB::Cursor * cursor = instance->db->cursor();
  NanReturnValue(ExportCursor::New(cursor, instance));
}

