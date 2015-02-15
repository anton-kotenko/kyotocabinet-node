#ifndef PROTO_HASH_DB_DEFINED
#define PROTO_HASH_DB_DEFINED

#include <kcpolydb.h>
#include "export_to_v8.h"

class ExportProtoHashDB: public ExportToV8<kyotocabinet::ProtoHashDB> {
};

#endif
