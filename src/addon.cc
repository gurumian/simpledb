#include <napi.h>
#include "connection_wrap.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return Connection::Init(env, exports);
}