#include "prepared_statement_wrap.h"
#include <napi.h>
#include <uv.h>
#include "log_message.h"

Napi::FunctionReference PreparedStatement::constructor;

Napi::Object PreparedStatement::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env,
                  "PreparedStatement",
                  {
                    InstanceMethod("execute", &PreparedStatement::Execute),
                    InstanceMethod("setString", &PreparedStatement::SetString),
                    InstanceMethod("setInt", &PreparedStatement::SetInt),
                    InstanceMethod("setInt64", &PreparedStatement::SetInt64),
                    InstanceMethod("setDouble", &PreparedStatement::SetDouble),
                    InstanceMethod("setBlob", &PreparedStatement::SetBlob),
                    InstanceMethod("executeUpdate", &PreparedStatement::ExecuteUpdate),
                  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("PreparedStatement", func);

  return exports;
}

PreparedStatement::PreparedStatement(const Napi::CallbackInfo& info) : Napi::ObjectWrap<PreparedStatement>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() <= 0 || !info[0].IsExternal()) {
    Napi::TypeError::New(env, "External expected").ThrowAsJavaScriptException();
    return;
  }

  auto stat = info[0].As<Napi::External<util::db::PreparedStatement>>();
  stat_ = std::unique_ptr<util::db::PreparedStatement>{stat.Data()};
  assert(stat_);
}

Napi::Object PreparedStatement::NewInstance(Napi::Env env, Napi::Value arg) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({arg});
  return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value PreparedStatement::Execute(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  auto deferred = Napi::Promise::Deferred::New(env);
  bool res = stat_->Execute();
  if(res) {
    deferred.Resolve(Napi::Boolean::New(env, res));
  }
  else {
    deferred.Reject(Napi::Error::New(env, "Error on executing").Value());
  }
  return deferred.Promise();
}


void PreparedStatement::SetString(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() <= 0 || !info[0].IsObject()) {
    Napi::TypeError::New(env, "Object expected").ThrowAsJavaScriptException();
    return;
  }

  Napi::Object obj = info[0].ToObject();
  if(!obj.HasOwnProperty("index")) {
    Napi::TypeError::New(env, "no index").ThrowAsJavaScriptException();
    return;
  }

  if(!static_cast<Napi::Value>(obj["index"]).IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return;
  }
  int index = (int) static_cast<Napi::Value>(obj["index"]).ToNumber();

  if(!obj.HasOwnProperty("value")) {
    Napi::TypeError::New(env, "no value").ThrowAsJavaScriptException();
    return;
  }

  if(!static_cast<Napi::Value>(obj["value"]).IsString()) {
    Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    return;
  }

  Napi::String value = static_cast<Napi::Value>(obj["value"]).ToString();
  stat_->SetString(index, value.Utf8Value());
}

void PreparedStatement::SetInt(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() <= 0 || !info[0].IsObject()) {
    Napi::TypeError::New(env, "Object expected").ThrowAsJavaScriptException();
    return;
  }

  Napi::Object obj = info[0].ToObject();
  if(!obj.HasOwnProperty("index")) {
    Napi::TypeError::New(env, "no index").ThrowAsJavaScriptException();
    return;
  }

  if(!static_cast<Napi::Value>(obj["index"]).IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return;
  }
  int index = (int) static_cast<Napi::Value>(obj["index"]).ToNumber();

  if(!obj.HasOwnProperty("value")) {
    Napi::TypeError::New(env, "no value").ThrowAsJavaScriptException();
    return;
  }

  if(!static_cast<Napi::Value>(obj["value"]).IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return;
  }
  int value = (int) static_cast<Napi::Value>(obj["value"]).ToNumber();
  stat_->SetInt(index, value);
}

void PreparedStatement::SetInt64(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() <= 0 || !info[0].IsObject()) {
    Napi::TypeError::New(env, "Object expected").ThrowAsJavaScriptException();
    return;
  }

  Napi::Object obj = info[0].ToObject();
  if(!obj.HasOwnProperty("index")) {
    Napi::TypeError::New(env, "no index").ThrowAsJavaScriptException();
    return;
  }

  if(!static_cast<Napi::Value>(obj["index"]).IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return;
  }
  int index = (int) static_cast<Napi::Value>(obj["index"]).ToNumber();

  if(!obj.HasOwnProperty("value")) {
    Napi::TypeError::New(env, "no value").ThrowAsJavaScriptException();
    return;
  }

  if(!static_cast<Napi::Value>(obj["value"]).IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return;
  }
  int64_t value = (int64_t) static_cast<Napi::Value>(obj["value"]).ToNumber();
  stat_->SetInt(index, value);
}

void PreparedStatement::SetDouble(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() <= 0 || !info[0].IsObject()) {
    Napi::TypeError::New(env, "Object expected").ThrowAsJavaScriptException();
    return;
  }

  Napi::Object obj = info[0].ToObject();
  if(!obj.HasOwnProperty("index")) {
    Napi::TypeError::New(env, "no index").ThrowAsJavaScriptException();
    return;
  }

  if(!static_cast<Napi::Value>(obj["index"]).IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return;
  }
  int index = (int) static_cast<Napi::Value>(obj["index"]).ToNumber();

  if(!obj.HasOwnProperty("value")) {
    Napi::TypeError::New(env, "no value").ThrowAsJavaScriptException();
    return;
  }

  if(!static_cast<Napi::Value>(obj["value"]).IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return;
  }
  double value = (double) static_cast<Napi::Value>(obj["value"]).ToNumber();
  stat_->SetDouble(index, value);
}

void PreparedStatement::SetBlob(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() <= 0 || !info[0].IsObject()) {
    Napi::TypeError::New(env, "Object expected").ThrowAsJavaScriptException();
    return;
  }

  Napi::Object obj = info[0].ToObject();
  if(!obj.HasOwnProperty("index")) {
    Napi::TypeError::New(env, "no index").ThrowAsJavaScriptException();
    return;
  }

  if(!static_cast<Napi::Value>(obj["index"]).IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return;
  }
  int index = (int) static_cast<Napi::Value>(obj["index"]).ToNumber();

  if(!obj.HasOwnProperty("value")) {
    Napi::TypeError::New(env, "no value").ThrowAsJavaScriptException();
    return;
  }

  if(!static_cast<Napi::Value>(obj["value"]).IsArrayBuffer()) {
    Napi::TypeError::New(env, "ArrayBuffer expected").ThrowAsJavaScriptException();
    return;
  }

  Napi::ArrayBuffer value = static_cast<Napi::Value>(obj["value"]).As<Napi::ArrayBuffer>();
  stat_->SetBlob(index, value.Data(), value.ByteLength());
}

Napi::Value PreparedStatement::ExecuteUpdate(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  auto deferred = Napi::Promise::Deferred::New(env);
  bool res = stat_->ExecuteUpdate();
  if(res) {
    deferred.Resolve(Napi::Boolean::New(env, res));
  }
  else {
    deferred.Reject(Napi::Error::New(env, "Error on executing").Value());
  }
  return deferred.Promise();
}