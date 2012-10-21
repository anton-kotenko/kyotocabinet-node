#ifndef STANDART_PROGRESS_CHECKER_DEFINED
#define STANDART_PROGRESS_CHECKER_DEFINED 1
#include <node/uv.h>
#include <iostream>
using namespace std;
template <class T> class standartProgressChecker: public kyotocabinet::BasicDB::ProgressChecker {
 private:
  class Message {
    public:
      char * name;
      char * message;
      int64_t curcnt;
      int64_t allcnt; 
      uv_sem_t message_lock;
      bool result;
  };
  standartProgressChecker::Message msg;
  v8::Persistent<v8::Arguments> _argv;
  v8::Persistent<v8::Object> _callbacks;
  uv_async_t message_notification;
  uv_work_t work;
  T * _db; 
  static void message_notification_cb (uv_async_t * handle, int status);
  static void work_cb(uv_work_t * req);
  static void after_work_cb(uv_work_t * req);  
 public:
  standartProgressChecker(T * db, v8::Handle<v8::Object> argv);
  bool check(const char * name, const char * message, int64_t curcnt, int64_t allcnt);
  void run();
  ~standartProgressChecker();
};
template <class T> standartProgressChecker<T>::standartProgressChecker(T * db, v8::Handle<v8::Object> argv) {
  this->_argv = v8::Persistent<v8::Arguments>::New(argv); 
  this->message_notification.data = this;
  this->work.data = this;
  this->_db = db;
  uv_sem_init(&this->msg.message_lock, 0);
  uv_async_init(uv_default_loop(), &this->message_notification, standartProgressChecker::message_notification_cb);
  if (this->_argv.Length() < 1 || !this->_argv[this->_argv.Length() -1]->IsObject()) {
    return;
  }
  this->_callbacks = v8::Persistent<v8::Object>::New(this->_argv[this->_argv.Length() - 1]); 
}
template <class T> standartProgressChecker<T>::~standartProgressChecker() {
  cerr<<"progress checker destructor"<<endl;
  uv_sem_destroy(&this->msg.message_lock);
  this->_argv.Dispose();
  this->_callbacks->Dispose();
}
template <class T> bool standartProgressChecker<T>::check(const char * name, const char * message, int64_t curcnt, int64_t allcnt) {
  this->msg.name = const_cast<char *>(name);
  this->msg.message = const_cast<char *>(message);
  this->msg.curcnt = curcnt;
  this->msg.allcnt = allcnt; 
  this->msg.result = true;
  cerr<<"send"<<endl;
  uv_async_send(&this->message_notification);
  cerr<<"send ok"<<endl;
  cerr<<"sleep 2"<<endl;
  uv_sem_wait(&this->msg.message_lock);
  cerr<<"after sleep"<<endl;
  return this->msg.result;
}
template <class T> void standartProgressChecker<T>::message_notification_cb (uv_async_t * handle, int status) {
  v8::HandleScope scope;
  standartProgressChecker * this_obj = static_cast<standartProgressChecker*>(handle->data);
  v8::Handle<v8::Value> argv[4];
  argv[0] = v8::String::New(this_obj->msg.name);
  argv[1] = v8::String::New(this_obj->msg.message);
  argv[2] = v8::Number::New(this_obj->msg.curcnt);
  argv[3] = v8::Number::New(this_obj->msg.allcnt);
  if (!this_obj->_callbacks->Has(v8::String::New("check"))){
    this_obj->msg.result = true;
    uv_sem_post(&this_obj->msg.message_lock);
    return;
  }
  v8::Handle<v8::Value> callback = this_obj->_callbacks->Get(v8::String::New("check")); 
  if (!callback->IsFunction()) {
    this_obj->msg.result = true;
    uv_sem_post(&this_obj->msg.message_lock);
    return;
  }
  this_obj->msg.result = v8::Function::Cast(*callback)->Call(this_obj->_progressChecker, 4, argv)->BooleanValue();
  uv_sem_post(&this_obj->msg.message_lock);
}
template <class T> void standartProgressChecker<T>::run() {
    uv_queue_work(uv_default_loop(), &this->work, work_cb, after_work_cb);
}
template <class T> void standartProgressChecker<T>::work_cb(uv_work_t * req) {
  standartProgressChecker<T> * this_obj = static_cast<standartProgressChecker<T>* >(req->data);
}
template <class T> void standartProgressChecker<T>::after_work_cb(uv_work_t * req) {
  v8::HandleScope scope;
  standartProgressChecker<T> * this_obj = static_cast<standartProgressChecker<T>* >(req->data);
  if (!this_obj->_callbacks->Has(v8::String::New("end"))){
    return;
  }
  v8::Handle<v8::Value> callback = this_obj->_callbacks->Get(v8::String::New("end")); 
  if (!callback->IsFunction()) {
    return;
  }
  v8::Function::Cast(*callback)->Call(this_obj->_callbacks, 0, NULL);
}
#endif //STANDART_PROGRESS_CHECKER_DEFINED
