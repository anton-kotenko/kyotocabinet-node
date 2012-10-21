v8::Persistent<v8::FunctionTemplate> ExportCursor::constructor_tpl_;
ExportCursor::ExportCursor () {
  this->cursor_ = NULL;
}
ExportCursor::~ExportCursor () {
  if(this->cursor_) {
    delete this->cursor_;
  }
}
v8::Handle<v8::Value> ExportCursor::New(const v8::Arguments & args) {
  (new ExportCursor())->Wrap(args.This());
  return args.This();
}
v8::Handle<v8::Value> ExportCursor::New(kyotocabinet::DB::Cursor * cursor) {
  SetPrototype();
  v8::Local<v8::Function> fcn = constructor_tpl_->GetFunction();
  v8::Local<v8::Object> cursorObj = fcn->NewInstance();
  node::ObjectWrap::Unwrap<ExportCursor>(cursorObj)->cursor_ = cursor;
  return cursorObj;
}
void ExportCursor::SetPrototype() {
  if (!ExportCursor::constructor_tpl_.IsEmpty()) {
    return;
  }
  ExportCursor::constructor_tpl_ = v8::Persistent<v8::FunctionTemplate>(v8::FunctionTemplate::New(ExportCursor::New));
  constructor_tpl_->SetClassName(v8::String::NewSymbol("cursor")); 
  constructor_tpl_->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(constructor_tpl_, "accept", ExportCursor::accept);
  NODE_SET_PROTOTYPE_METHOD(constructor_tpl_, "set_value", ExportCursor::set_value);
  NODE_SET_PROTOTYPE_METHOD(constructor_tpl_, "remove", ExportCursor::remove);
  NODE_SET_PROTOTYPE_METHOD(constructor_tpl_, "get_key", ExportCursor::get_key);
  NODE_SET_PROTOTYPE_METHOD(constructor_tpl_, "get_value", ExportCursor::get_value);
  NODE_SET_PROTOTYPE_METHOD(constructor_tpl_, "get", ExportCursor::get);
  NODE_SET_PROTOTYPE_METHOD(constructor_tpl_, "jump", ExportCursor::jump);
  NODE_SET_PROTOTYPE_METHOD(constructor_tpl_, "jump_back", ExportCursor::jump_back);
  NODE_SET_PROTOTYPE_METHOD(constructor_tpl_, "step", ExportCursor::step);
  NODE_SET_PROTOTYPE_METHOD(constructor_tpl_, "step_back", ExportCursor::step_back);
}
v8::Handle<v8::Value> ExportCursor::accept(const v8::Arguments & args) {
  v8::HandleScope scope;
  v8::Local<v8::Object> v8_visitor; 
  bool writable = true, step = false;
  REQUIRE_OBJECT(args, 0, v8_visitor, "Cursor.accept requires first argument to be visitor object");
  if (args.Length() > 1) {
    REQUIRE_BOOLEAN(args, 1, writable, "Cursor.accept requires second argument to be boolean or to be abscent");
  }
  if (args.Length() > 2) {
    REQUIRE_BOOLEAN(args, 2, step, "Cursor.accept requires third argument to be boolean or to be abscent");
  }
  ExportCursor * instance = node::ObjectWrap::Unwrap<ExportCursor>(args.This());  
  ExportVisitor visitor(v8_visitor);
  return scope.Close(v8::Boolean::New(instance->cursor_->accept(&visitor, writable, step)));
}
v8::Handle<v8::Value> ExportCursor::set_value(const v8::Arguments & args) {
  v8::HandleScope scope;
  v8::Local<v8::Object> value;
  bool step = false;
  REQUIRE_BUFFER(args, 0, value, "Cursor.set_value requires first argument to be Buffer");
  if (args.Length() > 1) {
    REQUIRE_BOOLEAN(args, 1, step, "Curstor.set_value requires second argument to be boolean or to be abscent");
  }
  ExportCursor * instance = node::ObjectWrap::Unwrap<ExportCursor>(args.This());  
  return scope.Close(v8::Boolean::New(instance->cursor_->set_value(BUFFER_DATA_LENGTH(value), step)));
}
v8::Handle<v8::Value> ExportCursor::remove(const v8::Arguments & args) {
  ExportCursor * instance = node::ObjectWrap::Unwrap<ExportCursor>(args.This());  
  return v8::Boolean::New(instance->cursor_->remove());
}
v8::Handle<v8::Value> ExportCursor::get_key(const v8::Arguments & args) {
  v8::HandleScope scope;
  char * key_data;
  size_t key_length;
  bool step = false;
  if (args.Length() > 0) {
    REQUIRE_BOOLEAN(args, 0, step, "Cursor.get_key requires first argument to be boolean or not to be at all");
  }
  ExportCursor * instance = node::ObjectWrap::Unwrap<ExportCursor>(args.This());  
  key_data = instance->cursor_->get_key(&key_length, step);
  if (!key_data) {
    return scope.Close(v8::Undefined());
  }
  v8::Local<v8::Object> key = v8::Local<v8::Object>::New(node::Buffer::New(key_data, key_length)->handle_);
  delete key_data;
  return scope.Close(key);
}
v8::Handle<v8::Value> ExportCursor::get_value(const v8::Arguments & args) {
  v8::HandleScope scope;
  char * value_data;
  size_t value_length;
  bool step = false;
  if (args.Length() > 0) {
    REQUIRE_BOOLEAN(args, 0, step, "Cursor.get_value requires first argument to be boolean or not to be at all");
  }
  ExportCursor * instance = node::ObjectWrap::Unwrap<ExportCursor>(args.This());  
  value_data = instance->cursor_->get_value(&value_length, step);
  if (!value_data) {
    return scope.Close(v8::Undefined());
  }
  v8::Local<v8::Object> value = v8::Local<v8::Object>::New(node::Buffer::New(value_data, value_length)->handle_);
  delete value_data;
  return scope.Close(value);

}
v8::Handle<v8::Value> ExportCursor::get(const v8::Arguments & args) {
  v8::HandleScope scope;
  char * value_data = NULL, * key_data = NULL;
  size_t value_length, key_length;
  bool step = false;
  if (args.Length() > 0) {
    REQUIRE_BOOLEAN(args, 0, step, "Cursor.get requires first argument to be boolean or not to be at all");
  }
  ExportCursor * instance = node::ObjectWrap::Unwrap<ExportCursor>(args.This());  
  key_data = instance->cursor_->get(&key_length, const_cast<const char **>(&value_data), &value_length,  step);
  if (!key_data) {
    return scope.Close(v8::Undefined());
  }
  v8::Local<v8::Object> value = v8::Local<v8::Object>::New(node::Buffer::New(value_data, value_length)->handle_);
  if (value_data) {
    //for some reason i can't delete this memory region
    //possibly it points to internal, private memory region of database
    //and database maintain it by itself without me
    //delete value_data;
  }
  v8::Local<v8::Object> key = v8::Local<v8::Object>::New(node::Buffer::New(key_data, key_length)->handle_);
  if (key_data) {
    delete key_data;
  }
  v8::Local<v8::Object> pair = v8::Object::New();
  pair->Set(v8::String::New("key"), key);
  pair->Set(v8::String::New("value"), value);
  return scope.Close(pair);
}
v8::Handle<v8::Value> ExportCursor::jump(const v8::Arguments & args) {
  v8::HandleScope scope;
  v8::Local<v8::Object> key;
  ExportCursor * instance = node::ObjectWrap::Unwrap<ExportCursor>(args.This());  
  if (args.Length() > 0) {
    REQUIRE_BUFFER(args, 0, key, "Cursor.jump requires first argument to be buffer or to be abscent");
    return scope.Close(v8::Boolean::New(instance->cursor_->jump(BUFFER_DATA_LENGTH(key))));
  }
  return scope.Close(v8::Boolean::New(instance->cursor_->jump()));
}
v8::Handle<v8::Value> ExportCursor::jump_back(const v8::Arguments & args) {
  v8::HandleScope scope;
  v8::Local<v8::Object> key;
  ExportCursor * instance = node::ObjectWrap::Unwrap<ExportCursor>(args.This());  
  if (args.Length() > 0) {
    REQUIRE_BUFFER(args, 0, key, "Cursor.jump_back requires first argument to be buffer or to be abscent");
    return scope.Close(v8::Boolean::New(instance->cursor_->jump_back(BUFFER_DATA_LENGTH(key))));
  }
  return scope.Close(v8::Boolean::New(instance->cursor_->jump_back()));
}
v8::Handle<v8::Value> ExportCursor::step(const v8::Arguments & args) {
  v8::HandleScope scope;
  ExportCursor * instance = node::ObjectWrap::Unwrap<ExportCursor>(args.This());  
  return scope.Close(v8::Boolean::New(instance->cursor_->step()));
}
v8::Handle<v8::Value> ExportCursor::step_back(const v8::Arguments & args) {
  v8::HandleScope scope;
  ExportCursor * instance = node::ObjectWrap::Unwrap<ExportCursor>(args.This());  
  return scope.Close(v8::Boolean::New(instance->cursor_->step_back()));
}
