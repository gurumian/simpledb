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
                    InstanceMethod("next", &ResultSet::Next),
                    InstanceMethod("getInt", &ResultSet::GetInt),
                    InstanceMethod("getInt64", &ResultSet::GetInt64),
                    InstanceMethod("getString", &ResultSet::GetString),
                    InstanceMethod("getDouble", &ResultSet::GetDouble),
                    InstanceMethod("getBlob", &ResultSet::GetBlob),
                    InstanceAccessor("data", &ResultSet::Get, nullptr),
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
  res_ = std::unique_ptr<util::db::ResultSet>{res.Data()};
  assert(res_);
}

Napi::Object ResultSet::NewInstance(Napi::Env env, Napi::Value arg) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({arg});
  return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value ResultSet::Next(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::Boolean::New(info.Env(), res_->Next());
}

Napi::Value ResultSet::GetInt(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  auto index = (int)info[0].ToNumber();
  return Napi::Number::New(info.Env(), res_->GetInt(index));
}

Napi::Value ResultSet::GetString(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  auto index = (int)info[0].ToNumber();
  return Napi::String::New(info.Env(), res_->GetString(index));

}

Napi::Value ResultSet::GetInt64(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  auto index = (int)info[0].ToNumber();
  return Napi::Number::New(info.Env(), res_->GetInt64(index));

}

Napi::Value ResultSet::GetDouble(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  auto index = (int)info[0].ToNumber();
  return Napi::Number::New(info.Env(), res_->GetDouble(index));
}

Napi::Value ResultSet::GetBlob(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  auto index = (int)info[0].ToNumber();
  auto blob = res_->GetBlob(index);
  uint8_t *data = new uint8_t[blob->size()];
  std::copy(blob->begin(), blob->end(), data);
  assert(blob);

  return Napi::ArrayBuffer::New(env, data, (size_t) blob->size(), [](Napi::Env env, void *externalData){
    delete (uint8_t *)externalData;
  });
}

Napi::Value ResultSet::Get(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  int count = res_->GetColumeCount();
  auto data = Napi::Array::New(env, count);

  for(int i = 0; i < count; i++) {
    int type = res_->columnType(i);
    if(type == SQLITE_INTEGER) {
      data[i] = Napi::Number::New(env, res_->GetInt(i));
    }
    else if(type == SQLITE_FLOAT) {
      data[i] = Napi::Number::New(env, res_->GetDouble(i));
    }
    else if(type == SQLITE_TEXT) {
      data[i] = Napi::String::New(env, res_->GetString(i));
    }
    else if(type == SQLITE_BLOB) {
      auto blob = res_->GetBlob(i);
      uint8_t *buf = new uint8_t[blob->size()];
      std::copy(blob->begin(), blob->end(), buf);
      assert(blob);

      data[i] = Napi::ArrayBuffer::New(env, buf, (size_t) blob->size(), [](Napi::Env env, void *externalData){
        delete (uint8_t *)externalData;
      });
    }
    else if(type == SQLITE_NULL) {
      data[i] = env.Null();
    }
    else {
      LOG(WARNING) << __func__ << " unhandled: " << type;
    }
  }

  return data;
}