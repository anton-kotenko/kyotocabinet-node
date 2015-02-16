#ifndef DIR_DB_DEFINED
#define DIR_DB_DEFINED

#include <kcpolydb.h>
#include "export_to_v8.h"

class ExportDirDB: public ExportToV8<kyotocabinet::DirDB> {
};

#endif
