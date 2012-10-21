#include<node.h>
#include<node_version.h>
#include<v8.h>
#include<map>
#include<string>
#include<vector>
#include<iostream>
#include<typeinfo>
#include<kcpolydb.h>
#include "exporthashdb.hh"
#include "exportcachedb.hh"
#include "exportdirdb.hh"
#include "exportforestdb.hh"
#include "exportgrassdb.hh"
#include "exportpolydb.hh"
#include "exportprotohashdb.hh"
#include "exportprototreedb.hh"
#include "exportstashdb.hh"
#include "exporttextdb.hh"
#include "exporttreedb.hh"
#define EXPORT_TO_NODE(target, type) Export##type::Init(target, #type);
extern "C" {
void init (v8::Handle<v8::Object> target) {
  EXPORT_TO_NODE(target, PolyDB);
  EXPORT_TO_NODE(target, HashDB);
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

