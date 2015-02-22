#ifndef GRASS_DB_DEFINED
#define GRASS_DB_DEFINED

#include <kcpolydb.h>
#include "export_to_v8.h"

class ExportGrassDB: public ExportToV8<kyotocabinet::GrassDB> {
};

#endif
