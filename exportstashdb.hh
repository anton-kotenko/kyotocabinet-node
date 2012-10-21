#ifndef EXPORT_STASH_DB_DEFINED
#include "exporttov8.hh"
class ExportStashDB: public ExportToV8<kyotocabinet::StashDB> {
};
#define EXPORT_STASH_DB_DEFINED 1
#endif //EXPORT_STASH_DB_DEFINED
