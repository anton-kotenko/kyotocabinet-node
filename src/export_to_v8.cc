#include <string>
#include <map>
#include <vector>

#include <v8.h>

#include "tools.h"
#include "progress_checker.h"

template <class T> v8::Local<v8::FunctionTemplate> ExportToV8<T>::PrepareObjectTemlate(const char * exportName) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(ExportToV8<T>::New);
  tpl->SetClassName(Nan::New<v8::String>(exportName).ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  ExportToV8<T>::setDefaultPrototype(tpl);  
  return tpl;
}

template <class T> void  ExportToV8<T>::ExportTemplate(v8::Handle<v8::Object> target, v8::Handle<v8::FunctionTemplate> tpl, const char * exportName) {
  target->Set(Nan::New<v8::String>(exportName).ToLocalChecked(), tpl->GetFunction());
}

template <class T> void  ExportToV8<T>::Init(v8::Handle<v8::Object> target, const char * exportName) {
  v8::Local<v8::FunctionTemplate> tpl = PrepareObjectTemlate(exportName);
  ExportTemplate(target, tpl, exportName); 
};

template <class T> NAN_METHOD(ExportToV8<T>::New) {
  ExportToV8<T> * instance = new ExportToV8<T>();
  instance->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
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
  Nan::SetPrototypeMethod(tpl, "open", ExportToV8<T>::open);
  Nan::SetPrototypeMethod(tpl, "close", ExportToV8<T>::close);
  Nan::SetPrototypeMethod(tpl, "size", ExportToV8<T>::size);
  Nan::SetPrototypeMethod(tpl, "count", ExportToV8<T>::count);
  Nan::SetPrototypeMethod(tpl, "path", ExportToV8<T>::path);
  Nan::SetPrototypeMethod(tpl, "status", ExportToV8<T>::status);
  Nan::SetPrototypeMethod(tpl, "error", ExportToV8<T>::error);

  Nan::SetPrototypeMethod(tpl, "clear", ExportToV8<T>::clear);
  Nan::SetPrototypeMethod(tpl, "copy", ExportToV8<T>::copy);

  Nan::SetPrototypeMethod(tpl, "get", ExportToV8<T>::get);
  Nan::SetPrototypeMethod(tpl, "set", ExportToV8<T>::set);
  Nan::SetPrototypeMethod(tpl, "add", ExportToV8<T>::add);
  Nan::SetPrototypeMethod(tpl, "replace", ExportToV8<T>::replace);
  Nan::SetPrototypeMethod(tpl, "append", ExportToV8<T>::append);
  Nan::SetPrototypeMethod(tpl, "remove", ExportToV8<T>::remove);
  Nan::SetPrototypeMethod(tpl, "check", ExportToV8<T>::check);
  Nan::SetPrototypeMethod(tpl, "increment", ExportToV8<T>::increment);
  Nan::SetPrototypeMethod(tpl, "seize", ExportToV8<T>::seize);
  Nan::SetPrototypeMethod(tpl, "cas", ExportToV8<T>::cas);

  Nan::SetPrototypeMethod(tpl, "set_bulk", ExportToV8<T>::set_bulk);
  Nan::SetPrototypeMethod(tpl, "remove_bulk", ExportToV8<T>::remove_bulk);
  Nan::SetPrototypeMethod(tpl, "get_bulk", ExportToV8<T>::get_bulk);
  Nan::SetPrototypeMethod(tpl, "begin_transaction", ExportToV8<T>::begin_transaction);
  Nan::SetPrototypeMethod(tpl, "begin_transaction_try", ExportToV8<T>::begin_transaction_try);
  Nan::SetPrototypeMethod(tpl, "end_transaction", ExportToV8<T>::end_transaction);

  Nan::SetPrototypeMethod(tpl, "cursor", ExportToV8<T>::cursor);

  v8::Local<v8::ObjectTemplate> OpenMode = Nan::New<v8::ObjectTemplate>();
  #define DECL_MODE(MODE) OpenMode->Set(\
    Nan::New<v8::String>(#MODE).ToLocalChecked(),\
    Nan::New<v8::Int32>(T::MODE)\
  )
  DECL_MODE(OREADER);
  DECL_MODE(OWRITER);
  DECL_MODE(OCREATE);
  DECL_MODE(OTRUNCATE);
  DECL_MODE(OAUTOTRAN);
  DECL_MODE(OAUTOSYNC);
  DECL_MODE(ONOLOCK);
  DECL_MODE(OTRYLOCK);
  DECL_MODE(ONOREPAIR);
  #undef DECL_MODE
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("OpenMode").ToLocalChecked(), OpenMode);

  v8::Local<v8::ObjectTemplate> ErrorCode = Nan::New<v8::ObjectTemplate>();

  #define DECL_ERROR_CODE(CODE) ErrorCode->Set(\
    Nan::New<v8::String>(#CODE).ToLocalChecked(),\
    Nan::New<v8::Int32>(T::Error::CODE)\
  )
  DECL_ERROR_CODE(SUCCESS);
  DECL_ERROR_CODE(NOIMPL);
  DECL_ERROR_CODE(INVALID);
  DECL_ERROR_CODE(NOREPOS);
  DECL_ERROR_CODE(NOPERM);
  DECL_ERROR_CODE(BROKEN);
  DECL_ERROR_CODE(NOREC);
  DECL_ERROR_CODE(DUPREC);
  DECL_ERROR_CODE(LOGIC);
  DECL_ERROR_CODE(SYSTEM);
  DECL_ERROR_CODE(MISC);
  #undef DECL_ERROR_CODE
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("ErrorCode").ToLocalChecked(), ErrorCode);
}

template <class T> NAN_METHOD(ExportToV8<T>::open) {
  std::string path;
  int mode;
  REQUIRE_STRING(info, 0, path, "open requires first argument to be string")
  REQUIRE_INT32(info, 1, mode, "open requires second argument to be integer")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->open(path, mode)));
}

template <class T> NAN_METHOD(ExportToV8<T>::close) {
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->close()));
}

template <class T> NAN_METHOD(ExportToV8<T>::size) {
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  //FIXME that's BAD: what if size is greater than uint_32?
  info.GetReturnValue().Set(Nan::New<v8::Integer>(static_cast<uint32_t>(instance->db->size())));
}

template <class T> NAN_METHOD(ExportToV8<T>::count) {
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  //FIXME that's BAD: what if count is greater than uint_32?
  info.GetReturnValue().Set(Nan::New<v8::Integer>(static_cast<uint32_t>(instance->db->count())));
}

template <class T> NAN_METHOD(ExportToV8<T>::path) {
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  info.GetReturnValue().Set(Nan::New<v8::String>(instance->db->path().c_str()).ToLocalChecked());
}

template <class T> NAN_METHOD(ExportToV8<T>::status) {
  std::map<std::string, std::string> valueMap;
  v8::Local<v8::Object> value = Nan::New<v8::Object>();
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  if(!instance->db->status(&valueMap)) {
    return info.GetReturnValue().Set(Nan::Undefined());
  }
  for(std::map<std::string, std::string>::const_iterator i = valueMap.begin(); i!=valueMap.end(); i++ ){
    value->Set(
        Nan::New<v8::String>(i->first.c_str()).ToLocalChecked(),
        Nan::New<v8::String>(i->second.c_str()).ToLocalChecked()
      );
  }
  info.GetReturnValue().Set(value);
}

template <class T> NAN_METHOD(ExportToV8<T>::error) {
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  typename T::Error kcError = instance->db->error();
  v8::Local<v8::Object> v8Error = v8::Local<v8::Object>::Cast(v8::Exception::Error(Nan::New<v8::String>(kcError.message()).ToLocalChecked()));
  v8Error->Set(
    Nan::New<v8::String>("name").ToLocalChecked(),
    Nan::New<v8::String>(kcError.name()).ToLocalChecked()
  );
  v8Error->Set(
    Nan::New<v8::String>("code").ToLocalChecked(),
    Nan::New<v8::Int32>(kcError.code())
  );
  info.GetReturnValue().Set(v8Error);
}

template <class T> NAN_METHOD(ExportToV8<T>::clear) {
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->clear()));
}

template <class T> NAN_METHOD(ExportToV8<T>::copy) {
  std::string path;
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  //v8::Local<v8::Object> callback = info[1]->ToObject();
  StandartProgressChecker * checker = new StandartProgressChecker(/*callback*/);
  REQUIRE_STRING(info, 0, path, "copy requires first argument to be string")
  bool result = instance->db->copy(path, checker);
  delete checker;
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}


template <class T> NAN_METHOD(ExportToV8<T>::get) {
  std::string key;
  std::string value; 
  REQUIRE_STRING(info, 0, key, "get requires first argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  if (!instance->db->get(key, &value)) {
    return info.GetReturnValue().Set(Nan::Undefined());
  } 
  info.GetReturnValue().Set(Nan::New<v8::String>(value.c_str()).ToLocalChecked());
}

template <class T> NAN_METHOD(ExportToV8<T>::set) {
  std::string key;
  std::string value; 
  REQUIRE_STRING(info, 0, key, "set requires first argument to be string")
  REQUIRE_STRING(info, 1, value, "set requires second argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->set(key, value))); 
}

template <class T> NAN_METHOD(ExportToV8<T>::add) {
  std::string key;
  std::string value; 
  REQUIRE_STRING(info, 0, key, "add requires first argument to be string")
  REQUIRE_STRING(info, 1, value, "add requires second argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->add(key, value))); 
}

template <class T> NAN_METHOD(ExportToV8<T>::replace) {
  std::string key;
  std::string value; 
  REQUIRE_STRING(info, 0, key, "replace requires first argument to be string")
  REQUIRE_STRING(info, 1, value, "replace requires second argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->replace(key, value))); 
}

template <class T> NAN_METHOD(ExportToV8<T>::append) {
  std::string key;
  std::string value; 
  REQUIRE_STRING(info, 0, key, "append requires first argument to be string")
  REQUIRE_STRING(info, 1, value, "append requires second argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->append(key, value))); 
}

template <class T> NAN_METHOD(ExportToV8<T>::remove) {
  std::string key;
  REQUIRE_STRING(info, 0, key, "remove requires first argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->remove(key)));
}

template <class T> NAN_METHOD(ExportToV8<T>::check) {
  std::string key;
  REQUIRE_STRING(info, 0, key, "check requires first argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  info.GetReturnValue().Set(Nan::New<v8::Int32>(instance->db->check(key)));
}

template <class T> NAN_METHOD(ExportToV8<T>::increment) {
  std::string key;
  int64_t intIncrement;
  int64_t intOriginal = 0;
  double doubleIncrement;
  double doubleOriginal = 0;
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  REQUIRE_STRING(info, 0, key, "increment requires first argument to be string")
  if (info.Length()<2) {
    return info.GetReturnValue().Set(V8_THROW(v8::Exception::Error(
      Nan::New<v8::String>("increment function requires at least 2 arguments").ToLocalChecked()
    )));
  }
  if (info[1]->IsInt32()) {
    REQUIRE_INT64(info, 1, intIncrement, "increments second argument is not integer")
    if (info.Length()>=3) {
      REQUIRE_INT64(info, 2, intOriginal, "increments third argument has to be integer if first is integer")
    }
    return info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->increment(key, intIncrement, intOriginal)));
  } else if(info[1]->IsNumber()) {
    REQUIRE_DOUBLE(info, 1, doubleIncrement, "increments second argument is not double")
    if (info.Length()>=3) {
      REQUIRE_INT64(info, 2, doubleOriginal, "increments third argument has to be double if first is double")
    }
    return info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->increment_double(key, doubleIncrement, doubleOriginal)));
  } 

  info.GetReturnValue().Set(V8_THROW(v8::Exception::Error(
    Nan::New<v8::String>("increment function requires second argument to be integer or double").ToLocalChecked()
  )));
}

template <class T> NAN_METHOD(ExportToV8<T>::seize) {
  std::string key;
  std::string value; 
  REQUIRE_STRING(info, 0, key, "seize requires first argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  if (!instance->db->seize(key, &value)) {
    return info.GetReturnValue().Set(Nan::Undefined());
  }
  info.GetReturnValue().Set(Nan::New<v8::String>(value.c_str()).ToLocalChecked()); 
}

template <class T> NAN_METHOD(ExportToV8<T>::cas) {
  std::string key;
  std::string ovalue; 
  std::string nvalue; 
  REQUIRE_STRING(info, 0, key, "cas requires first argument to be string")
  REQUIRE_STRING(info, 1, ovalue, "cas requires second argument to be string")
  REQUIRE_STRING(info, 2, nvalue, "cas requires third argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->cas(key, ovalue, nvalue))); 
}

template <class T> NAN_METHOD(ExportToV8<T>::set_bulk) {
  std::map<std::string, std::string> data;
  bool atomic = true;
  v8::Local<v8::Object> bulk; 
  v8::Local<v8::Array> keys;  
  REQUIRE_OBJECT(info, 0, bulk, "set_bulk requires first argument to be object")
  if (info.Length()>=2) {
    REQUIRE_BOOLEAN(info, 1, atomic, "set_bulk requires second argument to be boolean")
  }
  keys = bulk->GetOwnPropertyNames();
  for ( unsigned int i=0; i<keys->Length(); i++) {
    data[*v8::String::Utf8Value(keys->Get(i))] = *v8::String::Utf8Value(bulk->Get(keys->Get(i)));
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  //FIXME that's BAD: what if size is greater than uint_32?
  info.GetReturnValue().Set(Nan::New<v8::Integer>(static_cast<uint32_t>(instance->db->set_bulk(data, atomic))));
}

template <class T> NAN_METHOD(ExportToV8<T>::remove_bulk) {
  std::vector<std::string> data;
  bool atomic = true;
  v8::Local<v8::Array> keys;  
  REQUIRE_ARRAY(info, 0, keys, "remove_bulk requires first argument to be array")
  if (info.Length()>=2) {
    REQUIRE_BOOLEAN(info, 1, atomic, "remove_bulk requires second argument to be boolean")
  }
  data.resize(keys->Length());
  for ( unsigned int i=0; i<keys->Length(); i++) {
    data[i] = *v8::String::Utf8Value(keys->Get(i));
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  //FIXME that's BAD: what if size is greater than uint_32?
  info.GetReturnValue().Set(Nan::New<v8::Integer>(static_cast<uint32_t>(instance->db->remove_bulk(data, atomic)))); 
}

template <class T> NAN_METHOD(ExportToV8<T>::get_bulk) {
  std::vector<std::string> keysVector;
  std::map<std::string, std::string> valuesMap;
  bool atomic = true;
  v8::Local<v8::Array> keys;  
  v8::Local<v8::Object> values = Nan::New<v8::Object>();

  REQUIRE_ARRAY(info, 0, keys, "get_bulk requires first argument to be array")
  if (info.Length()>=2) {
    REQUIRE_BOOLEAN(info, 1, atomic, "get_bulk requires second argument to be boolean")
  }
  keysVector.resize(keys->Length());
  for ( unsigned int i=0; i<keys->Length(); i++) {
    keysVector[i] = *v8::String::Utf8Value(keys->Get(i));
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  if(-1 == instance->db->get_bulk(keysVector, &valuesMap, atomic)) {
    return info.GetReturnValue().Set(Nan::Undefined());
  }
  for (std::map<std::string, std::string>::const_iterator i = valuesMap.begin(); i != valuesMap.end(); i++) {
    values->Set(
      Nan::New<v8::String>(i->first.c_str()).ToLocalChecked(),
      Nan::New<v8::String>(i->second.c_str()).ToLocalChecked()
    );
  }
  info.GetReturnValue().Set(values);
}

template <class T> NAN_METHOD(ExportToV8<T>::begin_transaction) {
  bool hard = false;
  if (info.Length() > 0) {
    REQUIRE_BOOLEAN(info, 0, hard, "begin_transaction requires first argument to be boolean");
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->begin_transaction(hard)));
}

template <class T> NAN_METHOD(ExportToV8<T>::begin_transaction_try) {
  bool hard = false;
  if (info.Length() > 0) {
    REQUIRE_BOOLEAN(info, 0, hard, "begin_transaction_try requires first argument to be boolean");
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->begin_transaction_try(hard)));
}

template <class T> NAN_METHOD(ExportToV8<T>::end_transaction) {
  bool commit = true;
  if (info.Length() > 0) {
    REQUIRE_BOOLEAN(info, 0, commit, "end_transaction requires first argument to be boolean");
  }
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(instance->db->end_transaction(commit)));
}

template <class T> NAN_METHOD(ExportToV8<T>::cursor) {
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(info.This());
  kyotocabinet::DB::Cursor * cursor = instance->db->cursor();
  info.GetReturnValue().Set(ExportCursor::New(cursor, instance));
}

