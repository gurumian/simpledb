#include "result_set_wrap.h"
#include <napi.h>
#include <uv.h>
#include "log_message.h"

Napi::FunctionReference ResultSet::constructor;

Napi::Object ResultSet::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env,
                  "ResultSet",
                  {
                    // InstanceMethod("execute", &Statement::Execute),
                    // InstanceMethod("executeQuery", &Statement::ExecuteQuery),
                  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("ResultSet", func);

  return exports;
}

ResultSet::ResultSet(const Napi::CallbackInfo& info) : Napi::ObjectWrap<ResultSet>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);


  if (info.Length() <= 0 || !info[0].IsExternal()) {
    Napi::TypeError::New(env, "External expected").ThrowAsJavaScriptException();
    return;
  }

  auto res = info[0].As<Napi::External<util::db::ResultSet>>();
  res_ = res.Data();
  assert(res_);
}

ResultSet::~ResultSet() {
  LOG(INFO) << __func__;
  if(res_)
    delete res_;
  res_ = nullptr;
}

Napi::Object ResultSet::NewInstance(Napi::Env env, Napi::Value arg) {
  LOG(INFO) << __func__;
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({arg});
  return scope.Escape(napi_value(obj)).ToObject();
}