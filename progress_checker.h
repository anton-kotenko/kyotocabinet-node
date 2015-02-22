#ifndef STANDART_PROGRESS_CHECKER_DEFINED
#define STANDART_PROGRESS_CHECKER_DEFINED

#include <iostream>

#include <v8.h>

#include <kcpolydb.h>

#include "node_modules/nan/nan.h"

class StandartProgressChecker: public kyotocabinet::BasicDB::ProgressChecker {
 private:
  //v8::Persistent<v8::Object> callback;
 public:
  StandartProgressChecker(/*v8::Handle<v8::Object> _callback):callback(_callback*/) {
   // std::cerr<<"progress checker constructor thread:"<<gettid()<<std::endl;
  }
  bool check(const char * name, const char * message, int64_t curcnt, int64_t allcnt) {
    //sleep(10000);
    std::cerr<<"name: "<<name<<std::endl<<"message: "<<message<<std::endl<<"curcnt: "<<curcnt<<", allcnt: "<<allcnt<<std::endl;
    //std::cerr<<"progress checker checker thread:"<<gettid()<<std::endl;
    //v8::HandleScope scope;
    //v8::Local<v8::Object> ctx  = v8::Object::New();
    //v8::Function::Cast(*(this->callback->Get(NanNew<v8::String>("cb"))))->Call(this->callback, 0,  NULL);
    return true;
  }
  ~StandartProgressChecker() {
  } 
};

#endif
