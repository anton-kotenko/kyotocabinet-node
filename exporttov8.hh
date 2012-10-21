#ifndef EXPORT_TO_V8_DEFINED
#define EXPORT_TO_V8_DEFINED 1
#include "argv_helpers.hh"
#include "standart_progress_checker.hh"
#include "exportvisitor.hh"
#include <node/uv.h>
#include <node/node_buffer.h>
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
#include "exporttov8.cc"
#endif //EXPORT_TO_V8_DEFINED
