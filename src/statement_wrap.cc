#include "statement_wrap.h"
#include "result_set_wrap.h"
#include <napi.h>
#include <uv.h>
#include "log_message.h"

Napi::FunctionReference Statement::constructor;

Napi::Object Statement::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  constexpr auto name = "_Statement";

  Napi::Function func =
      DefineClass(env,
                  name,
                  {
                    InstanceMethod("execute", &Statement::Execute),
                    InstanceMethod("executeQuery", &Statement::ExecuteQuery),
                  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set(name, func);

  return exports;
}

Statement::Statement(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Statement>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);


  if (info.Length() <= 0 || !info[0].IsExternal()) {
    Napi::TypeError::New(env, "External expected").ThrowAsJavaScriptException();
    return;
  }

  auto stat = info[0].As<Napi::External<util::db::Statement>>();
  stat_ = std::unique_ptr<util::db::Statement>{stat.Data()};
  assert(stat_);
}

Napi::Object Statement::NewInstance(Napi::Env env, Napi::Value arg) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({arg});
  return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value Statement::Execute(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() <= 0 || !info[0].IsString()) {
    Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  auto query = info[0].ToString().Utf8Value();

  auto deferred = Napi::Promise::Deferred::New(env);
  assert(stat_);
  bool res = stat_->Execute(query);
  if(res) {
    deferred.Resolve(Napi::Boolean::New(env, res));
  }
  else {
    deferred.Reject(Napi::Error::New(env, "Error on executing").Value());
  }
  return deferred.Promise();
}

Napi::Value Statement::ExecuteQuery(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() <= 0 || !info[0].IsString()) {
    Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  auto query = info[0].ToString().Utf8Value();

  auto deferred = Napi::Promise::Deferred::New(env);
  assert(stat_);
  auto res = stat_->ExecuteQuery(query)->Unref();
  if(res) {
    deferred.Resolve(ResultSet::NewInstance(env, Napi::External<util::db::ResultSet>::New(env, res)));
  }
  else {
    deferred.Reject(Napi::Error::New(env, "Error on executing").Value());
  }
  return deferred.Promise();
}