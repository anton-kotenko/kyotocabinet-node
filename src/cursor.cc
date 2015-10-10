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
  ExportCursor * instance = new ExportCursor();
  instance->Wrap(info.This()); 
  info.GetReturnValue().Set(info.This());
}

v8::Handle<v8::Function> ExportCursor::Init() {
  v8::Local<v8::FunctionTemplate> f_tpl = Nan::New<v8::FunctionTemplate>(ExportCursor::JsConstructor);
  f_tpl->SetClassName(Nan::New<v8::String>("Cursor").ToLocalChecked());
  f_tpl->InstanceTemplate()->SetInternalFieldCount(1);
  Nan::SetPrototypeMethod(f_tpl, "set", ExportCursor::Set);
  Nan::SetPrototypeMethod(f_tpl, "remove", ExportCursor::Remove);
  Nan::SetPrototypeMethod(f_tpl, "get_key", ExportCursor::GetKey);
  Nan::SetPrototypeMethod(f_tpl, "get_value", ExportCursor::GetValue);
  Nan::SetPrototypeMethod(f_tpl, "get", ExportCursor::Get);
  Nan::SetPrototypeMethod(f_tpl, "jump", ExportCursor::Jump);
  Nan::SetPrototypeMethod(f_tpl, "jump_back", ExportCursor::JumpBack);
  Nan::SetPrototypeMethod(f_tpl, "step", ExportCursor::Step);
  Nan::SetPrototypeMethod(f_tpl, "step_back", ExportCursor::StepBack);
  return f_tpl->GetFunction();
}

v8::Local<v8::Object> ExportCursor::New (kyotocabinet::DB::Cursor * cursor, iRefable * parent_db) {
  Nan::EscapableHandleScope scope;
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
  return scope.Escape(thisObj); 
}

NAN_METHOD(ExportCursor::Set) {
  GET_CURSOR(info, cursor);
  std::string new_value;
  bool step = false;
  REQUIRE_STRING(info, 0, new_value, "Cursor.Set expects first argument to be string")
  if (info.Length() > 2) {
    REQUIRE_BOOLEAN(info, 1, step, "Cursor.Set expects second argument to be boolean or to be abscent");
  }
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(cursor->set_value_str(new_value, step)));
};

NAN_METHOD(ExportCursor::Remove) {
  GET_CURSOR(info, cursor);
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(cursor->remove()));
};

NAN_METHOD(ExportCursor::GetKey) {
  GET_CURSOR(info, cursor);
  std::string key;
  bool step = false;
  if (info.Length() > 0) {
    REQUIRE_BOOLEAN(info, 0, step, "Cursor.GetKey expects first argument to be boolean or to be abscent");
  }
  if (!cursor->get_key(&key, step)) {
    return info.GetReturnValue().Set(Nan::Undefined());
  }
  info.GetReturnValue().Set(Nan::New<v8::String>(key.c_str()).ToLocalChecked());
};

NAN_METHOD(ExportCursor::GetValue) {
  GET_CURSOR(info, cursor);
  std::string value;
  bool step = false;
  if (info.Length() > 0) {
    REQUIRE_BOOLEAN(info, 0, step, "Cursor.GetValue expects first argument to be boolean or to be abscent");
  }
  if (!cursor->get_value(&value, step)) {
    return info.GetReturnValue().Set(Nan::Undefined());
  }

  info.GetReturnValue().Set(Nan::New<v8::String>(value.c_str()).ToLocalChecked());
};

NAN_METHOD(ExportCursor::Get) {
  GET_CURSOR(info, cursor);
  std::string value;
  std::string key;
  bool step = false;
  v8::Local<v8::Object> result = Nan::New<v8::Object>();
  if (info.Length() > 0) {
    REQUIRE_BOOLEAN(info, 0, step, "Cursor.Get expects first argument to be boolean or to be abscent");
  }
  if (cursor->get(&key, &value, step)) {
    result->Set(
      Nan::New<v8::String>("key").ToLocalChecked(),
      Nan::New<v8::String>(key.c_str()).ToLocalChecked()
    );
    result->Set(
      Nan::New<v8::String>("value").ToLocalChecked(),
      Nan::New<v8::String>(value.c_str()).ToLocalChecked()
    );
    info.GetReturnValue().Set(result);
  } else {
    info.GetReturnValue().Set(Nan::Undefined());
  }
};

NAN_METHOD(ExportCursor::Jump) {
  GET_CURSOR(info, cursor);
  std::string key;
  bool result; 
  if (info.Length() > 0) {
    REQUIRE_STRING(info, 0, key, "Cursor.Jump requires first argument to be string or to be abscent");
    result = cursor->jump(key);
  } else {
    result = cursor->jump();
  }
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
};

NAN_METHOD(ExportCursor::JumpBack) {
  GET_CURSOR(info, cursor);
  std::string key;
  bool result; 
  if (info.Length() > 0) {
    REQUIRE_STRING(info, 0, key, "Cursor.JumpBack requires first argument to be string or to be abscent");
    result = cursor->jump_back(key);
  } else {
    result = cursor->jump_back();
  }
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
};

NAN_METHOD(ExportCursor::Step) {
  GET_CURSOR(info, cursor);
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(cursor->step())); 
};

NAN_METHOD(ExportCursor::StepBack) {
  GET_CURSOR(info, cursor);
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(cursor->step_back())); 
};

