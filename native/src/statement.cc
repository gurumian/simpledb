#include "simpledb.h"
#include <unistd.h>
#include "log_message.h"

namespace util {
namespace db {

Statement::Statement(const sqlite3* conn)
: conn_(conn) {
}

Statement::Statement(const sqlite3* conn, std::unique_ptr<sqlite3_stmt, std::function<void(sqlite3_stmt *)>> stmt)
: Statement(conn) {
  stmt_ = std::move(stmt);
  assert(stmt_);
}

bool Statement::Execute(const std::string& sql) {
  return ExecuteUpdate(sql);
}

bool Statement::ExecuteUpdate(const std::string &sql) {
  int err;
  err = Prepare(sql);
  if(err) {
    return false;
  }

  for(int ret = 0; (ret = sqlite3_step(stmt_.get())) != SQLITE_DONE;) {
    if(ret == SQLITE_BUSY) {
      sleep(1);
    }
    else {
      return false;
    }
  }
  return true;
}

std::unique_ptr<ResultSet> Statement::ExecuteQuery(const std::string &sql) {
  int err{0};
  err = Prepare(sql);
  if(err) {
    LOG(ERROR) << __func__ << " failed to Prepare()";
    return nullptr;
  }

  std::unique_ptr<ResultSet> result{new ResultSet(std::move(stmt_))};
  return result;
}

int Statement::Prepare(const std::string &sql) {
  sqlite3_stmt *stmt{};
  assert(conn_);
  int err = sqlite3_prepare_v2((sqlite3*)conn_, sql.c_str(), sql.length(), &stmt, NULL);
  if(err) {
    throw std::runtime_error(sqlite3_errmsg((sqlite3*)conn_));
  }

  stmt_ = std::unique_ptr<sqlite3_stmt, std::function<void(sqlite3_stmt *)>> {
    stmt,
    [](sqlite3_stmt *ptr){ if(ptr) sqlite3_finalize(ptr);}
  };

  return err;
}

Statement *Statement::Unref() {
  return new Statement(conn_, std::move(stmt_));
}

} // namespace db

} // namespace util
