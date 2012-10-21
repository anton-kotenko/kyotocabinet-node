#ifndef EXPORT_PROTO_TREE_DB_DEFINED
#include "exporttov8.hh"
class ExportProtoTreeDB: public ExportToV8<kyotocabinet::ProtoTreeDB> {
};
#define EXPORT_PROTO_TREE_DB_DEFINED 1
#endif //EXPORT_PROTO_TREE_DB_DEFINED
