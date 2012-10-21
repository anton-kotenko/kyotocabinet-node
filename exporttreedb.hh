#ifndef EXPORT_TREE_DB_DEFINED
#include "exporttov8.hh"
class ExportTreeDB: public ExportToV8<kyotocabinet::TreeDB> {
};
#define EXPORT_TREE_DB_DEFINED 1
#endif //EXPORT_TREE_DB_DEFINED
