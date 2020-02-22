#ifndef GURUM_CONNECTION_WRAP_H
#define GURUM_CONNECTION_WRAP_H

#include <napi.h>

#include "simpledb/simpledb.h"
#include <atomic>

class Connection : public Napi::ObjectWrap<Connection> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Connection(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;

  Napi::Value CreateStatement(void);

private:
  std::unique_ptr<util::db::Connection> connection_{};
  bool log_enabled_{false};
};

#endif // GURUM_CONNECTION_WRAP_H