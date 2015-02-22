#ifndef POLY_DB_DEFINED
#define POLY_DB_DEFINED

#include <kcpolydb.h>
#include "export_to_v8.h"

class ExportPolyDB : public ExportToV8<kyotocabinet::PolyDB> {
};

#endif
