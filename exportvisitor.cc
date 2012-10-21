//TODO: do i need to delete kbuf and vbuf???
ExportVisitor::ExportVisitor(v8::Handle<v8::Object> visitor):_visitor(visitor) {
}
ExportVisitor::~ExportVisitor() {
}
const char * ExportVisitor::visit_full(const char * kbuf, size_t ksiz, const char * vbuf, size_t vsiz, size_t * sp){
  v8::HandleScope scope;
  v8::Local<v8::String> full_sym = v8::String::New("full");
  v8::Handle<v8::Value> argv[2];
  if (!this->_visitor->Has(full_sym) || !this->_visitor->Get(full_sym)->IsFunction()) {
    return kyotocabinet::DB::Visitor::NOP;
  } 
  if (kbuf) {
    argv[0] = v8::Local<v8::Object>::New(node::Buffer::New(const_cast<char *>(kbuf), ksiz)->handle_);
  } else {
    argv[0] = v8::Null();
  }
  if (vbuf) {
    argv[1] = ((node::Buffer::New(const_cast<char *>(vbuf), vsiz)->handle_));
  } else {
    argv[1] = v8::Null();
  }
  
  v8::Handle<v8::Value> result = node::MakeCallback(this->_visitor, "full" , 2, argv);
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
  v8::HandleScope scope;
  v8::Local<v8::String> empty_sym = v8::String::New("empty");
  v8::Handle<v8::Value> argv[1];
  if (!this->_visitor->Has(empty_sym) || !this->_visitor->Get(empty_sym)->IsFunction()) {
    return kyotocabinet::DB::Visitor::NOP;
  } 
  if (kbuf) {
    argv[0] = v8::Handle<v8::Value>((node::Buffer::New(const_cast<char *>(kbuf), ksiz)->handle_));
  } else {
    argv[0] = v8::Null();
  }
  v8::Handle<v8::Value> result = node::MakeCallback(this->_visitor, empty_sym , 1, argv);
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
