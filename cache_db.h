#ifndef CACHE_DB_DEFINED
#define CACHE_DB_DEFINED

#include <kcpolydb.h>
#include "export_to_v8.h"

class ExportCacheDB: public ExportToV8<kyotocabinet::CacheDB> {
};

#endif
