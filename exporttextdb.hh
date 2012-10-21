#ifndef EXPORT_TEXT_DB_DEFINED
#include "exporttov8.hh"
class ExportTextDB: public ExportToV8<kyotocabinet::TextDB> {
};
#define EXPORT_TEXT_DB_DEFINED 1
#endif //EXPORT_TEXT_DB_DEFINED
