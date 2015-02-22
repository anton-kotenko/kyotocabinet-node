#ifndef FOREST_DB_DEFINED
#define FOREST_DB_DEFINED

#include <kcpolydb.h>
#include "export_to_v8.h"

class ExportForestDB: public ExportToV8<kyotocabinet::ForestDB> {
};

#endif
