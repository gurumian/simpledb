#include <napi.h>
#include "connection_wrap.h"
#include "statement_wrap.h"
#include "prepared_statement_wrap.h"
#include "result_set_wrap.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  ResultSet::Init(env, exports);
  PreparedStatement::Init(env, exports);
  Statement::Init(env, exports);
  return Connection::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)