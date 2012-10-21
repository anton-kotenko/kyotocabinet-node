#ifndef EXPORT_CACHE_DB_DEFINED
#include "exporttov8.hh"
class ExportCacheDB: public ExportToV8<kyotocabinet::CacheDB> {
};
#define EXPORT_CACHE_DB_DEFINED 1
#endif //EXPORT_CACHE_DB_DEFINED
