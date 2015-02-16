#ifndef PROTO_TREE_DB_DEFINED
#define PROTO_TREE_DB_DEFINED

#include <kcpolydb.h>
#include "export_to_v8.h"

class ExportProtoTreeDB: public ExportToV8<kyotocabinet::ProtoTreeDB> {
};

#endif
