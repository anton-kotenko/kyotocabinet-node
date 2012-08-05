#include<node.h>
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

template<class T> class ExportToV8: public node::ObjectWrap {
 public:     
  static void Init(v8::Handle<v8::Object> target);
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
  ExportToV8();
 private:
  T * db;
};
class standartProgressChecker: public kyotocabinet::BasicDB::ProgressChecker {
 private:
  v8::Local<v8::Function> callback;
  v8::Handle<v8::Object> ctx;
 public:
  standartProgressChecker(v8::Handle<v8::Object> ctx, v8::Local<v8::Function> callback) {
    this->callback = callback;
    this->ctx = ctx;
  }
  bool check(const char * name, const char * message, int64_t curcnt, int64_t allcnt) {
    std::cerr<<"name: "<<name<<std::endl<<"message: "<<message<<std::endl<<"curcnt: "<<curcnt<<", allcnt: "<<allcnt<<std::endl;
   // v8::HandleScope scope;
    //v8::Local<v8::Object> ctx  = v8::Object::New();
    //this->callback->Call(ctx, 0,  NULL);
    return true;
  }
  ~standartProgressChecker() {
  } 
};
class ExportPolyDb : public ExportToV8<kyotocabinet::PolyDB> {
};
class ExportHashDb: public ExportToV8<kyotocabinet::HashDB> {
};
class ExportTreeDb: public ExportToV8<kyotocabinet::TreeDB> {
};
class ExportDirDb: public ExportToV8<kyotocabinet::DirDB> {
};
class ExportForestDb: public ExportToV8<kyotocabinet::ForestDB> {
};
class ExportTextDb: public ExportToV8<kyotocabinet::TextDB> {
};
class ExportProtoHashDb: public ExportToV8<kyotocabinet::ProtoHashDB> {
};
class ExportProtoTreeDb: public ExportToV8<kyotocabinet::ProtoTreeDB> {
};
class ExportStashDb: public ExportToV8<kyotocabinet::StashDB> {
};
class ExportCacheDb: public ExportToV8<kyotocabinet::CacheDB> {
};
class ExportGrassDb: public ExportToV8<kyotocabinet::GrassDB> {
};
extern "C" {
void init (v8::Handle<v8::Object> target) {
  ExportPolyDb::Init(target);
  ExportHashDb::Init(target);
  ExportTreeDb::Init(target);
  ExportDirDb::Init(target);
  ExportForestDb::Init(target);
  ExportTextDb::Init(target);
  ExportProtoHashDb::Init(target);
  ExportProtoTreeDb::Init(target);
  ExportStashDb::Init(target);
  ExportCacheDb::Init(target);
  ExportGrassDb::Init(target);
}
}
NODE_MODULE(kyotonode, init);
template <class T> void  ExportToV8<T>::Init(v8::Handle<v8::Object> target) { 
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(ExportToV8<T>::New);
  tpl->SetClassName(v8::String::NewSymbol(typeid(T).name()));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  ExportToV8<T>::setDefaultPrototype(tpl);  
  v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
  target->Set(v8::String::NewSymbol(typeid(T).name()), constructor);
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
  node::SetPrototypeMethod(tpl, "open", ExportToV8<T>::open);
  node::SetPrototypeMethod(tpl, "close", ExportToV8<T>::close);
  node::SetPrototypeMethod(tpl, "size", ExportToV8<T>::size);
  node::SetPrototypeMethod(tpl, "count", ExportToV8<T>::count);
  node::SetPrototypeMethod(tpl, "path", ExportToV8<T>::path);
  node::SetPrototypeMethod(tpl, "clear", ExportToV8<T>::clear);
  node::SetPrototypeMethod(tpl, "get", ExportToV8<T>::get);
  node::SetPrototypeMethod(tpl, "set", ExportToV8<T>::set);
  node::SetPrototypeMethod(tpl, "add", ExportToV8<T>::add);
  node::SetPrototypeMethod(tpl, "replace", ExportToV8<T>::replace);
  node::SetPrototypeMethod(tpl, "remove", ExportToV8<T>::remove);
  node::SetPrototypeMethod(tpl, "check", ExportToV8<T>::check);
  node::SetPrototypeMethod(tpl, "seize", ExportToV8<T>::seize);
  node::SetPrototypeMethod(tpl, "cas", ExportToV8<T>::cas);
  node::SetPrototypeMethod(tpl, "increment", ExportToV8<T>::increment);
  node::SetPrototypeMethod(tpl, "append", ExportToV8<T>::append);
  node::SetPrototypeMethod(tpl, "set_bulk", ExportToV8<T>::set_bulk);
  node::SetPrototypeMethod(tpl, "remove_bulk", ExportToV8<T>::remove_bulk);
  node::SetPrototypeMethod(tpl, "get_bulk", ExportToV8<T>::get_bulk);
  node::SetPrototypeMethod(tpl, "status", ExportToV8<T>::status);
  node::SetPrototypeMethod(tpl, "copy", ExportToV8<T>::copy);
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
  if(args[1]->IsFunction()) {
    std::cerr<<"arg 1 is function"<<std::endl;
  }
  v8::Local<v8::Function> callback;
  callback->Call(args.This(), 0,NULL);
  standartProgressChecker checker(args.This(), callback);
  return scope.Close(v8::Boolean::New(instance->db->copy(path, &checker)));
}
