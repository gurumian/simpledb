#include "simpledb.h"
#include "log_message.h"
#include <unistd.h>

namespace util {
namespace db {

PreparedStatement::PreparedStatement(const sqlite3* connection, const std::string &sql) : Statement(connection) {
  int err = Prepare(sql);
  if(err) {
    LOG(FATAL) << " error(" << err << ")";
  }
}

PreparedStatement::PreparedStatement(const sqlite3* conn, std::unique_ptr<sqlite3_stmt, std::function<void(sqlite3_stmt *)>> stmt)
: Statement(conn, std::move(stmt)) {
}

void PreparedStatement::SetString(int i, const std::string &value) {
  int err = sqlite3_bind_text(stmt_.get(), i, value.c_str(), value.length(), SQLITE_TRANSIENT);
  if(err) {
    LOG(FATAL) << " error(" << err << ")";
  }
}

void PreparedStatement::SetInt(int i, int value) {
  int err = sqlite3_bind_int(stmt_.get(), i, value);
  if(err) {
    LOG(FATAL) << " error(" << err << ")";
  }
}

void PreparedStatement::SetInt(int i, int64_t value) {
  int err = sqlite3_bind_int64(stmt_.get(), i, value);
  if(err) {
    LOG(FATAL) << " error(" << err << ")";
  }
}

void PreparedStatement::SetDouble(int i, double value) {
  int err = sqlite3_bind_double(stmt_.get(), i, value);
  if(err) {
    LOG(FATAL) << " error(" << err << ")";
  }
}

void PreparedStatement::SetBlob(int i, void *value, size_t length) {
  int err = sqlite3_bind_blob(stmt_.get(), i, value, length, SQLITE_TRANSIENT);
  if(err) {
    LOG(FATAL) << " error(" << err << ")";
  }
}

bool PreparedStatement::Execute(void){
  return ExecuteUpdate();
}

bool PreparedStatement::ExecuteUpdate(void){
  for(int ret = 0; (ret = sqlite3_step(stmt_.get())) != SQLITE_DONE;) {
    if(ret == SQLITE_BUSY) {
      sleep(1);
      LOG(INFO) << " busy";
    }
    else {
      return false;
    }
  }
  return true;
}

std::unique_ptr<ResultSet>  PreparedStatement::ExecuteQuery(void){
  std::unique_ptr<ResultSet> result(new ResultSet(std::move(stmt_)));
  return result;
}

PreparedStatement *PreparedStatement::Unref() {
  return new PreparedStatement(conn_, std::move(stmt_));
}

} // namespace db

} // namespace util