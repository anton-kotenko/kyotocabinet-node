#ifndef TOOLS_DEFINED
#define TOOLS_DEFINED

#include <v8.h>

#include "node_modules/nan/nan.h"

/**
 * Warning: To understand what is happening here
 * take into account next fact
 * NanRetunXXX family are not functions. Thay are
 * macros like this
 * #define NanReturnXXX (...) return SOME_CODE_THAT_HANDLES_SCOPE_EXIT
 */
#if NODE_VERSION_AT_LEAST(0, 11, 0) 
#define V8_THROW(e) v8::Isolate::GetCurrent()->ThrowException(e)
#else
#define V8_THROW(e) ThrowException(e)
#endif

#define REQUIRE_STRING(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsString())\
    NanReturnValue(V8_THROW(v8::Exception::TypeError(NanNew<v8::String>(message))));\
  dst = *v8::String::Utf8Value(argv[num]->ToString());\
} while(0);
#define REQUIRE_INT32(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsInt32())\
    NanReturnValue(V8_THROW(v8::Exception::TypeError(NanNew<v8::String>(message))));\
  dst = static_cast<int>(argv[num]->Int32Value());\
} while(0);
#define REQUIRE_INT64(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsNumber())\
    NanReturnValue(V8_THROW(v8::Exception::TypeError(NanNew<v8::String>(message))));\
  dst = static_cast<int64_t>(argv[num]->IntegerValue());\
} while(0);
#define REQUIRE_DOUBLE(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsNumber())\
    NanReturnValue(V8_THROW(v8::Exception::TypeError(NanNew<v8::String>(message))));\
  dst = static_cast<double>(argv[num]->NumberValue());\
} while(0);
#define REQUIRE_OBJECT(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsObject())\
    NanReturnValue(V8_THROW(v8::Exception::TypeError(NanNew<v8::String>(message))));\
  dst = argv[num]->ToObject();\
} while(0);
#define REQUIRE_BOOLEAN(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsBoolean())\
    NanReturnValue(V8_THROW(v8::Exception::TypeError(NanNew<v8::String>(message))));\
  dst = static_cast<bool>(argv[num]->BooleanValue());\
} while(0);
#define REQUIRE_ARRAY(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsArray())\
    NanReturnValue(V8_THROW(v8::Exception::TypeError(NanNew<v8::String>(message))));\
  dst = v8::Local<v8::Array>::Cast(argv[num]);\
} while(0);
#define REQUIRE_INT8(argv, num, dst,  message)  do {\
  if (argv.Length()<=num || !argv[num]->IsInt32())\
    NanReturnValue(V8_THROW(v8::Exception::TypeError(NanNew<v8::String>(message))));\
  dst = static_cast<int8_t>(argv[num]->Int32Value());\
} while(0);

#endif
