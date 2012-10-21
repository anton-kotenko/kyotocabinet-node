#ifndef ARGV_HELPERS_INCLUDED
#define ARGV_HELPERS_INCLUDED 1
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
#define REQUIRE_BUFFER(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !node::Buffer::HasInstance(argv[num]))\
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New(message)));\
  dst = argv[num]->ToObject();\
} while(0);
#define BUFFER_DATA(buffer) node::Buffer::Data(buffer)
#define BUFFER_LENGTH(buffer) node::Buffer::Length(buffer)
#define BUFFER_DATA_LENGTH(buffer) node::Buffer::Data(buffer), node::Buffer::Length(buffer)
#endif //ARGV_HELPERS_INCLUDED

