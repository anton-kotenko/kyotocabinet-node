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
  static void cleanOnShutdown (void * args);
  static void registerShutdownCallback();
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
  static v8::Handle<v8::Value> accept(const v8::Arguments & args);
  static v8::Handle<v8::Value> accept_bulk(const v8::Arguments & args);
 protected:
  class databasesListNode {
   private:
    ExportToV8<T>::databasesListNode * next;
    ExportToV8<T>::databasesListNode * prev;
    ExportToV8<T> * item;
   public:
    ExportToV8<T>::databasesListNode * attachNew(ExportToV8<T> * _item){
      if (this->next != NULL) {
        return this->next->attachNew(_item);
      } 
      return this->next = new databasesListNode(this, _item);
    }
    ~databasesListNode() {
      if (this->prev) {
        this->prev->next = this->next;
      }
      if (this->next) {
        this->next->prev = this;
      }  
    }
    ExportToV8<T>::databasesListNode * getNext(){
      return this->next;
    }
    void deleteContainedItem() {
      if (this->item) {
        delete this->item;
        this->item = NULL;
      }
    }
    databasesListNode():next(NULL), prev(NULL), item(NULL){};
    databasesListNode(databasesListNode * _prev, ExportToV8<T> * _item): next(NULL), prev(_prev), item(_item){};
  };
  //ExportToV8<T> objects has to be created ONLY through ObjectWrap
  //this means, that instances of class will be created only by static method ExportToV8<T>::New
  virtual ~ExportToV8();
  ExportToV8();
  T * db;
  ExportToV8<T>::databasesListNode * me;
  static ExportToV8<T>::databasesListNode listRoot;
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
class ExportVisitor: public kyotocabinet::DB::Visitor {
 private:
  v8::Handle<v8::Object> _visitor; 
 public:
  ExportVisitor(v8::Handle<v8::Object> visitor);
  ~ExportVisitor();
  const char * visit_full(const char * kbuf, size_t ksiz, const char * vbuf, size_t vsize, size_t * sp);
  const char * visit_empty(const char * kbuf, size_t ksiz, size_t * sp);
  void visit_before();
  void visit_after();
};
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
ExportVisitor::ExportVisitor(v8::Handle<v8::Object> visitor):_visitor(visitor) {
}
ExportVisitor::~ExportVisitor() {
}
const char * ExportVisitor::visit_full(const char * kbuf, size_t ksiz, const char * vbuf, size_t vsiz, size_t * sp){
  v8::Local<v8::Value> full;
  v8::Local<v8::String> full_sym = v8::String::New("full");
  v8::Handle<v8::Value> argv[2];
  if (!this->_visitor->Has(full_sym)) {
    return kyotocabinet::DB::Visitor::NOP;
  } 
  full = this->_visitor->Get(full_sym);
  if (!full->IsFunction()) {
    return kyotocabinet::DB::Visitor::NOP;
  }
  argv[0] = kbuf ? v8::String::New(kbuf, ksiz) : v8::Null();
  argv[1] = vbuf ? v8::String::New(vbuf, vsiz) : v8::Null(); 
  v8::Local<v8::Value> result = v8::Function::Cast(*full)->Call(this->_visitor, 2, argv);
  if (result.IsEmpty() || result->IsUndefined()) {
    return kyotocabinet::DB::Visitor::NOP;
  }
  if (result->IsNull()) {
    return kyotocabinet::DB::Visitor::REMOVE;
  }
  if (!result->IsString()) {
    return kyotocabinet::DB::Visitor::NOP;
  }
  v8::String::Utf8Value resultString(result->ToString());
  *sp = resultString.length();
  return *resultString;
}
const char * ExportVisitor::visit_empty(const char * kbuf, size_t ksiz, size_t * sp) {
  v8::Local<v8::Value> empty;
  v8::Local<v8::String> empty_sym = v8::String::New("empty");
  v8::Handle<v8::Value> argv[1];
  if (!this->_visitor->Has(empty_sym)) {
    return kyotocabinet::DB::Visitor::NOP;
  } 
  empty = this->_visitor->Get(empty_sym);
  if (!empty->IsFunction()) {
    return kyotocabinet::DB::Visitor::NOP;
  }
  argv[0] = kbuf ? v8::String::New(kbuf, ksiz) : v8::Null();
  v8::Local<v8::Value> result = v8::Function::Cast(*empty)->Call(this->_visitor, 1, argv);
  if (result.IsEmpty() || result->IsUndefined()) {
    return kyotocabinet::DB::Visitor::NOP;
  }
  if (result->IsNull()) {
    return kyotocabinet::DB::Visitor::REMOVE;
  }
  if (!result->IsString()) {
    return kyotocabinet::DB::Visitor::NOP;
  }
  v8::String::Utf8Value resultString(result->ToString());
  *sp = resultString.length();
  return *resultString;
}
void ExportVisitor::visit_before() {
  v8::Local<v8::Value> before;
  v8::Local<v8::String> before_sym = v8::String::New("before");
  if (!this->_visitor->Has(before_sym)) {
    return;
  } 
  before = this->_visitor->Get(before_sym);
  if (!before->IsFunction()) {
    return;
  }
  v8::Function::Cast(*before)->Call(this->_visitor, 0, NULL);
}
void ExportVisitor::visit_after() {
  v8::Local<v8::Value> after;
  v8::Local<v8::String> after_sym = v8::String::New("after");
  if (!this->_visitor->Has(after_sym)) {
    return;
  } 
  after = this->_visitor->Get(after_sym);
  if (!after->IsFunction()) {
    return;
  }
  v8::Function::Cast(*after)->Call(this->_visitor, 0, NULL);
}
