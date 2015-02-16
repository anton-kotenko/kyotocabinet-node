#ifndef TREE_DB_DEFINED
#define TREE_DB_DEFINED

#include <kcpolydb.h>
#include "export_to_v8.h"

class ExportTreeDB: public ExportToV8<kyotocabinet::TreeDB> {
};

#endif
