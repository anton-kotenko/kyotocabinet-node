#ifndef EXPORT_PROTO_HASH_DB_DEFINED
#include "exporttov8.hh"
class ExportProtoHashDB: public ExportToV8<kyotocabinet::ProtoHashDB> {
};
#define EXPORT_PROTO_HASH_DB_DEFINED 1
#endif //EXPORT_PROTO_HASH_DB_DEFINED
