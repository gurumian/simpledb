#include "connection_wrap.h"
#include "statement_wrap.h"
#include "prepared_statement_wrap.h"
#include <napi.h>
#include <uv.h>
#include "log_message.h"
#include <memory>

Napi::FunctionReference Connection::constructor;

Napi::Object Connection::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env,
                  "Connection",
                  {
                    InstanceMethod("createStatement", &Connection::CreateStatement),
                    InstanceMethod("prepareStatement", &Connection::PrepareStatement),
                  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Connection", func);

  sqlite3_initialize();

  return exports;
}

Connection::Connection(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Connection>(info) {
  Napi::Env env = info.Env();

  if (info.Length() <= 0 || !info[0].IsString()) {
    Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    return;
  }

  connection_.reset(new util::db::Connection(info[0].ToString().Utf8Value()));
}

Napi::Value Connection::CreateStatement(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  auto stat = connection_->CreateStatement()->Clone();
  assert(stat);
  return Statement::NewInstance(env, Napi::External<util::db::Statement>::New(env, stat));
}

Napi::Value Connection::PrepareStatement(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() <= 0 || !info[0].IsString()) {
    Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  auto query = info[0].ToString().Utf8Value();

  auto stat = connection_->PrepareStatement(query)->Clone();
  assert(stat);
  return PreparedStatement::NewInstance(env, Napi::External<util::db::PreparedStatement>::New(env, stat));
}