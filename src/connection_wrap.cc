#include "connection_wrap.h"
#include <napi.h>
#include <uv.h>
#include "log_message.h"

Napi::FunctionReference Connection::constructor;

Napi::Object Connection::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env,
                  "Connection",
                  {
                    InstanceMethod("createStatement", &Connection::CreateStatement),
                  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Connection", func);

  return exports;
}

Connection::Connection(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Connection>(info) {
  if(log_enabled_) LOG(INFO) << __func__;
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  // connection_.reset(new util::db::Connection);
}

Napi::Value Connection::CreateStatement(void) {
  //
}