#ifndef GURUM_STATEMENT_WRAP_H
#define GURUM_STATEMENT_WRAP_H

#include <napi.h>

#include "simpledb/simpledb.h"
#include <atomic>

class Statement : public Napi::ObjectWrap<Statement> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Statement(const Napi::CallbackInfo& info);
  virtual ~Statement();

  static Napi::Object NewInstance(Napi::Env env, Napi::Value arg);

private:
  static Napi::FunctionReference constructor;

private:
  Napi::Value Execute(const Napi::CallbackInfo& info);
  Napi::Value ExecuteQuery(const Napi::CallbackInfo& info);

private:
  util::db::Statement *stat_{};
};

#endif // GURUM_STATEMENT_WRAP_H