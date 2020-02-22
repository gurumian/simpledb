#include "statement_wrap.h"
#include "result_set_wrap.h"
#include <napi.h>
#include <uv.h>
#include "log_message.h"

Napi::FunctionReference Statement::constructor;

Napi::Object Statement::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env,
                  "Statement",
                  {
                    InstanceMethod("execute", &Statement::Execute),
                    InstanceMethod("executeQuery", &Statement::ExecuteQuery),
                  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Statement", func);

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
  stat_ = stat.Data();
  assert(stat_);
}

Statement::~Statement() {
  LOG(INFO) << __func__;
  if(stat_)
    delete stat_;
  stat_ = nullptr;
}

Napi::Object Statement::NewInstance(Napi::Env env, Napi::Value arg) {
  LOG(INFO) << __func__;
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
  LOG(INFO) << __func__ << " query: " << query;

  assert(stat_);
  bool result = stat_->Execute(query);
  return Napi::Boolean::New(env, result);
}

Napi::Value Statement::ExecuteQuery(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() <= 0 || !info[0].IsString()) {
    Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  auto query = info[0].ToString().Utf8Value();
  LOG(INFO) << __func__ << " query: " << query;

  assert(stat_);
  auto res = stat_->ExecuteQuery(query)->Clone();
  assert(res);
  return ResultSet::NewInstance(env, Napi::External<util::db::ResultSet>::New(env, res));
}