#ifndef GURUM_RESULT_SET_WRAP_H
#define GURUM_RESULT_SET_WRAP_H

#include <napi.h>

#include "simpledb/simpledb.h"
#include <atomic>

class ResultSet : public Napi::ObjectWrap<ResultSet> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  ResultSet(const Napi::CallbackInfo& info);
  virtual ~ResultSet();

  static Napi::Object NewInstance(Napi::Env env, Napi::Value arg);

private:
  static Napi::FunctionReference constructor;

private:
  // Napi::Value Execute(const Napi::CallbackInfo& info);
  // Napi::Value ExecuteQuery(const Napi::CallbackInfo& info);

private:
  util::db::ResultSet *res_{};
};

#endif // GURUM_RESULT_SET_WRAP_H