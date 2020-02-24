#ifndef GURUM_RESULT_SET_WRAP_H
#define GURUM_RESULT_SET_WRAP_H

#include <napi.h>

#include "simpledb/simpledb.h"
#include <atomic>

class ResultSet : public Napi::ObjectWrap<ResultSet> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  ResultSet(const Napi::CallbackInfo& info);

  static Napi::Object NewInstance(Napi::Env env, Napi::Value arg);

private:
  static Napi::FunctionReference constructor;

private:
  Napi::Value Next(const Napi::CallbackInfo& info);
  Napi::Value GetInt(const Napi::CallbackInfo& info);
  Napi::Value GetString(const Napi::CallbackInfo& info);
  Napi::Value GetInt64(const Napi::CallbackInfo& info);
  Napi::Value GetDouble(const Napi::CallbackInfo& info);
  Napi::Value GetBlob(const Napi::CallbackInfo& info);
  Napi::Value Get(const Napi::CallbackInfo& info);
  Napi::Value GetAsObject(const Napi::CallbackInfo& info);

  Napi::ArrayBuffer blob(Napi::Env env, int index);

private:
  std::unique_ptr<util::db::ResultSet> res_{};
};

#endif // GURUM_RESULT_SET_WRAP_H