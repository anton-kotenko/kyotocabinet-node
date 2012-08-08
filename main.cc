#include<node.h>
#include<node_version.h>
#include<v8.h>
#include<map>
#include<string>
#include<vector>
#include<iostream>
#include<typeinfo>
#include<kcpolydb.h>
#define REQUIRE_STRING(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsString())\
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New(message)));\
  dst = *v8::String::Utf8Value(argv[num]->ToString());\
} while(0);
#define REQUIRE_INT32(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsInt32())\
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New(message)));\
  dst = static_cast<int>(argv[num]->Int32Value());\
} while(0);
#define REQUIRE_INT64(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsNumber())\
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New(message)));\
  dst = static_cast<int64_t>(argv[num]->IntegerValue());\
} while(0);
#define REQUIRE_DOUBLE(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsNumber())\
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New(message)));\
  dst = static_cast<double>(argv[num]->NumberValue());\
} while(0);
#define REQUIRE_OBJECT(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsObject())\
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New(message)));\
  dst = argv[num]->ToObject();\
} while(0);
#define REQUIRE_BOOLEAN(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsBoolean())\
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New(message)));\
  dst = static_cast<bool>(argv[num]->BooleanValue());\
} while(0);
#define REQUIRE_ARRAY(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsArray())\
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New(message)));\
  dst = v8::Array::Cast(*argv[num]);\
} while(0);
#define REQUIRE_INT8(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsInt32())\
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New(message)));\
  dst = static_cast<int8_t>(argv[num]->Int32Value());\
} while(0);

template<class T> class ExportToV8: public node::ObjectWrap {
 public:     
  static void Init(v8::Handle<v8::Object> target, const char * exportName);
  static void  ExportTemplate(v8::Handle<v8::Object> target, v8::Handle<v8::FunctionTemplate> tpl, const char * exportName);
  static v8::Local<v8::FunctionTemplate> PrepareObjectTemlate(const char * exportName);
  static v8::Handle<v8::Value> New(const v8::Arguments & args); 
  static void setDefaultPrototype(v8::Local<v8::FunctionTemplate> tpl);
  static v8::Handle<v8::Value> open(const v8::Arguments & args);
  static v8::Handle<v8::Value> close(const v8::Arguments & args);
  static v8::Handle<v8::Value> size(const v8::Arguments & args);
  static v8::Handle<v8::Value> count(const v8::Arguments & args);
  static v8::Handle<v8::Value> path(const v8::Arguments & args);
  static v8::Handle<v8::Value> clear(const v8::Arguments & args);
  static v8::Handle<v8::Value> set(const v8::Arguments & args);
  static v8::Handle<v8::Value> add(const v8::Arguments & args);
  static v8::Handle<v8::Value> replace(const v8::Arguments & args);
  static v8::Handle<v8::Value> append(const v8::Arguments & args);
  static v8::Handle<v8::Value> increment(const v8::Arguments & args);
  static v8::Handle<v8::Value> cas(const v8::Arguments & args);
  static v8::Handle<v8::Value> remove(const v8::Arguments & args);
  static v8::Handle<v8::Value> get(const v8::Arguments & args);
  static v8::Handle<v8::Value> check(const v8::Arguments & args);
  static v8::Handle<v8::Value> seize(const v8::Arguments & args);
  static v8::Handle<v8::Value> set_bulk(const v8::Arguments & args);
  static v8::Handle<v8::Value> remove_bulk(const v8::Arguments & args);
  static v8::Handle<v8::Value> get_bulk(const v8::Arguments & args);
  static v8::Handle<v8::Value> status(const v8::Arguments & args);
  static v8::Handle<v8::Value> copy(const v8::Arguments & args);
  static v8::Handle<v8::Value> begin_transaction(const v8::Arguments & args);
  static v8::Handle<v8::Value> begin_transaction_try(const v8::Arguments & args);
  static v8::Handle<v8::Value> end_transaction(const v8::Arguments & args);
  static v8::Handle<v8::Value> error(const v8::Arguments & args);
  ExportToV8();
 protected:
  T * db;
};
class standartProgressChecker: public kyotocabinet::BasicDB::ProgressChecker {
 private:
  v8::Persistent<v8::Object> callback;
 public:
  standartProgressChecker(v8::Handle<v8::Object> _callback):callback(_callback) {
   // std::cerr<<"progress checker constructor thread:"<<gettid()<<std::endl;
  }
  bool check(const char * name, const char * message, int64_t curcnt, int64_t allcnt) {
    //sleep(10000);
    std::cerr<<"name: "<<name<<std::endl<<"message: "<<message<<std::endl<<"curcnt: "<<curcnt<<", allcnt: "<<allcnt<<std::endl;
    //std::cerr<<"progress checker checker thread:"<<gettid()<<std::endl;
    v8::HandleScope scope;
    //v8::Local<v8::Object> ctx  = v8::Object::New();
    v8::Function::Cast(*(this->callback->Get(v8::String::New("cb"))))->Call(this->callback, 0,  NULL);
    return true;
  }
  ~standartProgressChecker() {
  } 
};
class ExportPolyDB : public ExportToV8<kyotocabinet::PolyDB> {
};
class ExportHashDB: public ExportToV8<kyotocabinet::HashDB> {
 public:
  static void Init(v8::Handle<v8::Object> target, const char * exportName);
  static void setCustomPrototype(v8::Local<v8::FunctionTemplate> tpl);
  static v8::Handle<v8::Value> tune_alignment(const v8::Arguments & args);
  static v8::Handle<v8::Value> tune_fbp(const v8::Arguments & args);
  static v8::Handle<v8::Value> tune_options(const v8::Arguments & args);
  static v8::Handle<v8::Value> tune_map(const v8::Arguments & args);
  static v8::Handle<v8::Value> tune_buckets(const v8::Arguments & args);
  static v8::Handle<v8::Value> tune_defrag(const v8::Arguments & args);
};
class ExportTreeDB: public ExportToV8<kyotocabinet::TreeDB> {
};
class ExportDirDB: public ExportToV8<kyotocabinet::DirDB> {
};
class ExportForestDB: public ExportToV8<kyotocabinet::ForestDB> {
};
class ExportTextDB: public ExportToV8<kyotocabinet::TextDB> {
};
class ExportProtoHashDB: public ExportToV8<kyotocabinet::ProtoHashDB> {
};
class ExportProtoTreeDB: public ExportToV8<kyotocabinet::ProtoTreeDB> {
};
class ExportStashDB: public ExportToV8<kyotocabinet::StashDB> {
};
class ExportCacheDB: public ExportToV8<kyotocabinet::CacheDB> {
};
class ExportGrassDB: public ExportToV8<kyotocabinet::GrassDB> {
};
#define EXPORT_TO_NODE(target, type) Export##type::Init(target, #type);
extern "C" {
void init (v8::Handle<v8::Object> target) {
  EXPORT_TO_NODE(target, PolyDB);
  EXPORT_TO_NODE(target, HashDB);
  EXPORT_TO_NODE(target, TreeDB);
  EXPORT_TO_NODE(target, DirDB);
  EXPORT_TO_NODE(target, ForestDB);
  EXPORT_TO_NODE(target, TextDB);
  EXPORT_TO_NODE(target, ProtoHashDB);
  EXPORT_TO_NODE(target, ProtoTreeDB);
  EXPORT_TO_NODE(target, StashDB);
  EXPORT_TO_NODE(target, CacheDB);
  EXPORT_TO_NODE(target, GrassDB);
}
}
NODE_MODULE(kyotonode, init);
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
template <class T> void  ExportToV8<T>::Init(v8::Handle<v8::Object> target, const char * exportName) {
  v8::HandleScope scope;
  v8::Local<v8::FunctionTemplate> tpl = PrepareObjectTemlate(exportName);
  ExportTemplate(target, tpl, exportName); 
};
template <class T> v8::Handle<v8::Value> ExportToV8<T>::New(const v8::Arguments & args) {
  ExportToV8<T> * instance = new ExportToV8<T>();
  instance->Wrap(args.This());
  return args.This();
}
template <class T> ExportToV8<T>::ExportToV8(){
  this->db = new T();
}
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
  return v8::String::New(value.c_str());
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::set(const v8::Arguments & args) {
  std::string key;
  std::string value; 
  REQUIRE_STRING(args, 0, key, "set requires first argument to be string")
  REQUIRE_STRING(args, 1, value, "set requires second argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Boolean::New(instance->db->set(key, value)); 
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::add(const v8::Arguments & args) {
  std::string key;
  std::string value; 
  REQUIRE_STRING(args, 0, key, "add requires first argument to be string")
  REQUIRE_STRING(args, 1, value, "add requires second argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Boolean::New(instance->db->add(key, value)); 
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::replace(const v8::Arguments & args) {
  std::string key;
  std::string value; 
  REQUIRE_STRING(args, 0, key, "replace requires first argument to be string")
  REQUIRE_STRING(args, 1, value, "replace requires second argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Boolean::New(instance->db->replace(key, value)); 
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::remove(const v8::Arguments & args) {
  std::string key;
  REQUIRE_STRING(args, 0, key, "remove requires first argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Boolean::New(instance->db->remove(key));
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::check(const v8::Arguments & args) {
  std::string key;
  REQUIRE_STRING(args, 0, key, "check requires first argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Int32::New(instance->db->check(key));
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::seize(const v8::Arguments & args) {
  std::string key;
  std::string value; 
  REQUIRE_STRING(args, 0, key, "seize requires first argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  if (!instance->db->seize(key, &value)) {
    return v8::Undefined(); 
  }
  return v8::String::New(value.c_str()); 
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::cas(const v8::Arguments & args) {
  std::string key;
  std::string ovalue; 
  std::string nvalue; 
  REQUIRE_STRING(args, 0, key, "cas requires first argument to be string")
  REQUIRE_STRING(args, 1, ovalue, "cas requires second argument to be string")
  REQUIRE_STRING(args, 2, nvalue, "cas requires third argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Boolean::New(instance->db->cas(key, ovalue, nvalue)); 
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::increment(const v8::Arguments & args) {
  std::string key;
  int64_t intIncrement;
  int64_t intOriginal = 0;
  double doubleIncrement;
  double doubleOriginal = 0;
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  REQUIRE_STRING(args, 0, key, "increment requires first argument to be string")
  if(args.Length()<2) {
    return v8::ThrowException(v8::Exception::Error(v8::String::New("increment function requires at least 2 arguments")));
  }
  if (args[1]->IsInt32()) {
    REQUIRE_INT64(args, 1, intIncrement, "increments second argument is not integer")
    if (args.Length()>=3) {
      REQUIRE_INT64(args, 2, intOriginal, "increments third argument has to be integer if first is integer")
    }
    return v8::Boolean::New(instance->db->increment(key, intIncrement, intOriginal));
  } else if(args[1]->IsNumber()) {
    REQUIRE_DOUBLE(args, 1, doubleIncrement, "increments second argument is not double")
    if (args.Length()>=3) {
      REQUIRE_INT64(args, 2, doubleOriginal, "increments third argument has to be double if first is double")
    }
    return v8::Boolean::New(instance->db->increment_double(key, doubleIncrement, doubleOriginal));
  } 
  return v8::ThrowException(v8::Exception::Error(v8::String::New("increment function requires second argument to be integer or double")));
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::append(const v8::Arguments & args) {
  std::string key;
  std::string value; 
  REQUIRE_STRING(args, 0, key, "append requires first argument to be string")
  REQUIRE_STRING(args, 1, value, "append requires second argument to be string")
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  return v8::Boolean::New(instance->db->append(key, value)); 
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::set_bulk(const v8::Arguments & args) {
  std::map<std::string, std::string> data;
  bool atomic = true;
  v8::HandleScope scope;
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
  return scope.Close(v8::Integer::New(instance->db->set_bulk(data, atomic)));
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::remove_bulk(const v8::Arguments & args) {
  std::vector<std::string> data;
  bool atomic = true;
  v8::HandleScope scope;
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
  return scope.Close(v8::Integer::New(instance->db->remove_bulk(data, atomic))); 
}
template <class T> v8::Handle<v8::Value> ExportToV8<T>::get_bulk(const v8::Arguments & args) {
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
template <class T> v8::Handle<v8::Value> ExportToV8<T>::copy(const v8::Arguments & args) {
  std::string path;
  ExportToV8<T> * instance = node::ObjectWrap::Unwrap<ExportToV8<T> >(args.This());
  v8::HandleScope scope;
  v8::Local<v8::Object> callback = args[1]->ToObject();
  standartProgressChecker checker(callback);
  REQUIRE_STRING(args, 0, path, "copy requires first argument to be string")
  return scope.Close(v8::Boolean::New(instance->db->copy(path, &checker)));
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
