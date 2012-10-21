#ifndef EXPORT_DIR_DB_DEFINED
#include "exporttov8.hh"
class ExportDirDB: public ExportToV8<kyotocabinet::DirDB> {
};
#define EXPORT_DIR_DB_DEFINED 1
#endif //EXPORT_DIR_DB_DEFINED
