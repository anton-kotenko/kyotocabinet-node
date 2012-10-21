#ifndef EXPORT_POLY_DB_DEFINED
#include "exporttov8.hh"
class ExportPolyDB : public ExportToV8<kyotocabinet::PolyDB> {
};
#define EXPORT_POLY_DB_DEFINED 1
#endif //EXPORT_POLY_DB_DEFINED
