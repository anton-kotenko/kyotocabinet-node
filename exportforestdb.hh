#ifndef EXPORT_FOREST_DB_DEFINED
#include "exporttov8.hh"
class ExportForestDB: public ExportToV8<kyotocabinet::ForestDB> {
};
#define EXPORT_FOREST_DB_DEFINED 1
#endif //EXPORT_FOREST_DB_DEFINED
