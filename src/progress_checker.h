#ifndef STANDART_PROGRESS_CHECKER_DEFINED
#define STANDART_PROGRESS_CHECKER_DEFINED

#include <iostream>

#include <v8.h>

#include <kcpolydb.h>

#include <nan/nan.h>

class StandartProgressChecker: public kyotocabinet::BasicDB::ProgressChecker {
 public:
  StandartProgressChecker() {
  }
  bool check(const char * name, const char * message, int64_t curcnt, int64_t allcnt) {
    return true;
  }
  ~StandartProgressChecker() {
  } 
};

#endif
