#ifndef EXPORT_VISITOR_DEFINED
#include<node/node_buffer.h>
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
#include "exportvisitor.cc"
#define EXPORT_VISITOR_DEFINED 1
#endif // EXPORT_VISITOR_DEFINED
