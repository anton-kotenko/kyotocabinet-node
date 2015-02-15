#include "node_modules/nan/nan.h"

#include "poly_db.h"
#include "hash_db.h"
#include "tree_db.h"
#include "dir_db.h"
#include "forest_db.h"
#include "text_db.h"
#include "proto_hash_db.h"
#include "proto_tree_db.h"
#include "stash_db.h"
#include "cache_db.h"
#include "proto_hash_db.h"
#include "grass_db.h"

#define EXPORT_TO_NODE(target, type) Export##type::Init(target, #type);
extern "C" {
void init (v8::Handle<v8::Object> target) {
  EXPORT_TO_NODE(target, PolyDB);
  //EXPORT_TO_NODE(target, HashDB);
  EXPORT_TO_NODE(target, TreeDB);
  EXPORT_TO_NODE(target, DirDB);
  EXPORT_TO_NODE(target, ForestDB);
  EXPORT_TO_NODE(target, TextDB);
  EXPORT_TO_NODE(target, ProtoHashDB);
  EXPORT_TO_NODE(target, ProtoTreeDB);
  EXPORT_TO_NODE(target, StashDB);
  EXPORT_TO_NODE(target, CacheDB);
  EXPORT_TO_NODE(target, GrassDB);
}
}
NODE_MODULE(kyotonode, init);
