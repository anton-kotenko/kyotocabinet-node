#ifndef EXPORT_GRASS_DB_DEFINED
#include "exporttov8.hh"
class ExportGrassDB: public ExportToV8<kyotocabinet::GrassDB> {
};
#define EXPORT_GRASS_DB_DEFINED 1
#endif //EXPORT_GRASS_DB_DEFINED
