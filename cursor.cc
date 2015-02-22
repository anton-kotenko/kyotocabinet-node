ExportCursor::ExportCursor() {
  cursor_ = NULL; 
  parent_db = NULL;
}

ExportCursor::~ExportCursor() {
  if (cursor_) {
    delete cursor_;
    cursor_ = NULL;
  }
  if (parent_db) {
    parent_db->Unref();
  }
}

NAN_METHOD(ExportCursor::JsConstructor) {
  NanScope();
  ExportCursor * instance = new ExportCursor();
  instance->Wrap(args.This()); 
  NanReturnThis();
}

v8::Handle<v8::Function> ExportCursor::Init() {
  v8::Local<v8::FunctionTemplate> f_tpl = NanNew<v8::FunctionTemplate>(ExportCursor::JsConstructor);
  f_tpl->SetClassName(NanNew<v8::String>("Cursor"));
  f_tpl->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(f_tpl, "set", ExportCursor::Set);
  NODE_SET_PROTOTYPE_METHOD(f_tpl, "remove", ExportCursor::Remove);
  NODE_SET_PROTOTYPE_METHOD(f_tpl, "get_key", ExportCursor::GetKey);
  NODE_SET_PROTOTYPE_METHOD(f_tpl, "get_value", ExportCursor::GetValue);
  NODE_SET_PROTOTYPE_METHOD(f_tpl, "get", ExportCursor::Get);
  NODE_SET_PROTOTYPE_METHOD(f_tpl, "jump", ExportCursor::Jump);
  NODE_SET_PROTOTYPE_METHOD(f_tpl, "jump_back", ExportCursor::JumpBack);
  NODE_SET_PROTOTYPE_METHOD(f_tpl, "step", ExportCursor::Step);
  NODE_SET_PROTOTYPE_METHOD(f_tpl, "step_back", ExportCursor::StepBack);
  return f_tpl->GetFunction();
}

v8::Handle<v8::Object> ExportCursor::New (kyotocabinet::DB::Cursor * cursor, iRefable * parent_db) {
  v8::Local<v8::Object> thisObj = ExportCursor::Init()->NewInstance();
  ExportCursor * cursor_inst = node::ObjectWrap::Unwrap<ExportCursor>(thisObj);
  cursor_inst->cursor_ = cursor;
  if (parent_db) {
    cursor_inst->parent_db = parent_db;
    //Mark database as used to avoid
    //situation when is removed by GC
    //while cursor is alive
    parent_db->Ref();
  }
  return thisObj; 
}

NAN_METHOD(ExportCursor::Set) {
  NanScope();
  GET_CURSOR(args, cursor);
  std::string new_value;
  bool step = false;
  REQUIRE_STRING(args, 0, new_value, "Cursor.Set expects first argument to be string")
  if (args.Length() > 2) {
    REQUIRE_BOOLEAN(args, 1, step, "Cursor.Set expects second argument to be boolean or to be abscent");
  }
  NanReturnValue(NanNew<v8::Boolean>(cursor->set_value_str(new_value, step)));
};

NAN_METHOD(ExportCursor::Remove) {
  NanScope();
  GET_CURSOR(args, cursor);
  NanReturnValue(NanNew<v8::Boolean>(cursor->remove()));
};

NAN_METHOD(ExportCursor::GetKey) {
  NanScope();
  GET_CURSOR(args, cursor);
  std::string key;
  bool step = false;
  if (args.Length() > 0) {
    REQUIRE_BOOLEAN(args, 0, step, "Cursor.GetKey expects first argument to be boolean or to be abscent");
  }
  if (!cursor->get_key(&key, step)) {
    NanReturnUndefined();
  }
  NanReturnValue(NanNew<v8::String>(key.c_str()));
};

NAN_METHOD(ExportCursor::GetValue) {
  NanScope();
  GET_CURSOR(args, cursor);
  std::string value;
  bool step = false;
  if (args.Length() > 0) {
    REQUIRE_BOOLEAN(args, 0, step, "Cursor.GetValue expects first argument to be boolean or to be abscent");
  }
  if (!cursor->get_value(&value, step)) {
    NanReturnUndefined();
  }
  NanReturnValue(NanNew<v8::String>(value.c_str()));
};

NAN_METHOD(ExportCursor::Get) {
  NanScope();
  GET_CURSOR(args, cursor);
  std::string value;
  std::string key;
  bool step = false;
  v8::Local<v8::Object> result = NanNew<v8::Object>();
  if (args.Length() > 0) {
    REQUIRE_BOOLEAN(args, 0, step, "Cursor.Get expects first argument to be boolean or to be abscent");
  }
  if (cursor->get(&key, &value, step)) {
    result->Set(NanNew<v8::String>("key"), NanNew<v8::String>(key.c_str()));
    result->Set(NanNew<v8::String>("value"), NanNew<v8::String>(value.c_str()));
    NanReturnValue(result);
  } else {
    NanReturnUndefined();
  }
};

NAN_METHOD(ExportCursor::Jump) {
  NanScope();
  GET_CURSOR(args, cursor);
  std::string key;
  bool result; 
  if (args.Length() > 0) {
    REQUIRE_STRING(args, 0, key, "Cursor.Jump requires first argument to be string or to be abscent");
    result = cursor->jump(key);
  } else {
    result = cursor->jump();
  }
  NanReturnValue(NanNew<v8::Boolean>(result));
};

NAN_METHOD(ExportCursor::JumpBack) {
  NanScope();
  GET_CURSOR(args, cursor);
  std::string key;
  bool result; 
  if (args.Length() > 0) {
    REQUIRE_STRING(args, 0, key, "Cursor.JumpBack requires first argument to be string or to be abscent");
    result = cursor->jump_back(key);
  } else {
    result = cursor->jump_back();
  }
  NanReturnValue(NanNew<v8::Boolean>(result));
};

NAN_METHOD(ExportCursor::Step) {
  NanScope();
  GET_CURSOR(args, cursor);
  NanReturnValue(NanNew<v8::Boolean>(cursor->step())); 
};

NAN_METHOD(ExportCursor::StepBack) {
  NanScope();
  GET_CURSOR(args, cursor);
  NanReturnValue(NanNew<v8::Boolean>(cursor->step_back())); 
};

