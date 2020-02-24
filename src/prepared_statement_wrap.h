#ifndef GURUM_PREPARED_STATEMENT_WRAP_H
#define GURUM_PREPARED_STATEMENT_WRAP_H

#include <napi.h>

#include "simpledb/simpledb.h"
#include <atomic>

class PreparedStatement : public Napi::ObjectWrap<PreparedStatement> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  PreparedStatement(const Napi::CallbackInfo& info);

  static Napi::Object NewInstance(Napi::Env env, Napi::Value arg);

private:
  static Napi::FunctionReference constructor;

private:
  Napi::Value Execute(const Napi::CallbackInfo& info);
  void SetString(const Napi::CallbackInfo& info);
  void SetInt(const Napi::CallbackInfo& info);
  void SetInt64(const Napi::CallbackInfo& info);
  void SetDouble(const Napi::CallbackInfo& info);
  void SetBlob(const Napi::CallbackInfo& info);
  Napi::Value ExecuteUpdate(const Napi::CallbackInfo& info);

  int index(const Napi::Object& obj);

private:
  std::unique_ptr<util::db::PreparedStatement> stat_{};
};

#endif // GURUM_PREPARED_STATEMENT_WRAP_H